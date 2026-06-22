#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"
#include "fmt/base.h"

#include "core/acceleration_structures/bvh.hpp"
#include "core/camera.hpp"
#include "core/material.hpp"
#include "core/primitive.hpp"
#include "core/render_target.hpp"
#include "core/resource_manager.h"
#include "importers/obj.hpp"
#include "core/scene.hpp"
#include <garbage/garbage_dump.hpp>

#include <core/vec.hpp>

#include <filesystem>
#include <algorithm>
#include <memory>
#include <vector> // TODO: Remove STL when bored

using namespace radiant;

int main(int argc, char* argv[])
{
    fmt::println("Starting Radiant");
    fmt::println("Argument count {}", argc);
    fmt::println("Arguments {}", *argv);

    std::filesystem::path destination;

    std::filesystem::path asset_source;

    if (argc >= 2)
    {
        destination = std::filesystem::path(argv[1]);
    }
    else
    {
        destination = std::filesystem::current_path().parent_path() / "render.png";
    }
    if (argc >= 3)
    {
        asset_source = std::filesystem::path(argv[2]);
    }
    else
    {
        asset_source = std::filesystem::path("../assets/cube/cube.obj");
    }

    SDL_Window*   window   = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture*  texture  = nullptr;

    SDL_SetAppMetadata("Radiant", "0.0", "radiant");

    u32 window_width  = 1366;
    u32 window_height = 768;

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());

    if (!SDL_CreateWindowAndRenderer(
            "Radiant", main_scale * window_width, main_scale * window_height, SDL_WINDOW_RESIZABLE, &window, &renderer))
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(renderer, window_width, window_height, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    CameraSettings settings{};
    settings.m_image_width       = 800;
    settings.m_samples_per_pixel = 1;
    settings.m_ray_bounces       = 3;
    settings.m_vfow_deg          = 20;

    settings.m_look_from      = vec3(-2.0f, 0.0f, 10.0f);
    settings.m_look_at        = vec3(0.0f, 0.0f, 0.0f);
    settings.m_world_up       = vec3(0.0f, 1.0f, 0.0f);
    settings.m_defocus_angle  = 0.1;
    settings.m_focus_distance = 3.4;

    Camera camera(settings);

    texture = SDL_CreateTexture(
        renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, settings.m_image_width, settings.m_image_height);
    if (!texture)
    {
        SDL_Log("Couldn't create streaming texture: %s", SDL_GetError());
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale); // Bake a fixed style scale. (until we have a solution for dynamic style scaling,
                                     // changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;

    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    u64   mesh_idx = ResourceManager::add_mesh(import_mesh(asset_source));
    Mesh& mesh     = ResourceManager::get_mesh(mesh_idx);

    std::vector<Shape> triangles;
    triangles.reserve(mesh.m_num_triangles);

    for (u32 t = 0; t < mesh.m_num_triangles; ++t)
    {
        triangles.emplace_back(make_triangle(mesh_idx, t));
    }

    fmt::println("Triangle count in an imported mesh {}", triangles.size());

    Shape ground_sphere = make_sphere(vec3(0.0, -101, -1.0), 100.0);
    Shape left_sphere   = make_sphere(vec3(-3.0, 1, 0.0), 2.0);
    Shape light_sphere  = make_sphere(vec3(3.2, 2, 0.0), 3);

    std::shared_ptr<Lambertian> material_ground = std::make_shared<Lambertian>(vec3(0.8f, 0.8f, 0.0f));
    std::shared_ptr<Lambertian> lambertian      = std::make_shared<Lambertian>(vec3(0.1f, 0.5f, 0.2f));
    std::shared_ptr<Lambertian> material_light  = std::make_shared<Lambertian>(vec3(1.0f, 1.0f, 1.0f));
    material_light->m_emissive                  = vec3(1.0f, 1.0f, 1.0f);
    std::shared_ptr<Dielectric> dielectric      = std::make_shared<Dielectric>(1.9);
    std::shared_ptr<Metal>      metal           = std::make_shared<Metal>(vec3(1.0, 1.0, 1.0), 0.05f);

    std::vector<Primitive> triangle_prims;
    triangle_prims.reserve(triangles.size());

    for (const auto& triangle : triangles)
    {
        triangle_prims.emplace_back(triangle, lambertian);
    }

    Primitive ground(ground_sphere, material_ground);
    Primitive left(left_sphere, metal);
    Primitive light(light_sphere, material_light);

    Scene s;
    s.m_materials = { material_light };

    BVH aggregate;
    // Populate the scene
    aggregate.insert(&ground);
    aggregate.insert(&left);
    aggregate.insert(&light);

    for (const auto& triangle_prim : triangle_prims)
    {
        aggregate.insert(&triangle_prim);
    }

    aggregate.build();

    // Render the scene to the image buffer
    RenderTarget render_target{};
    render_target.render_target.resize(settings.m_image_height * settings.m_image_width, {});
    render_target.width  = settings.m_image_width;
    render_target.height = settings.m_image_height;
    render_target.frame  = 1;

    SDL_Event event;
    bool      running   = true;
    bool      rendering = true;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
        }

        SDL_SetRenderScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColorFloat(renderer, 1, 1, 1, 1);
        SDL_RenderClear(renderer);

        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::Begin("Debug"); // Create a window called "Hello, world!" and append into it.
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            s.imgui_traverse();
            rendering = ImGui::Button(rendering ? "Pause" : "Resume") ? !rendering : rendering;
            ImGui::End();
        }
        if (s.m_dirty)
        {
            std::ranges::fill(render_target.render_target, vec3{});
            s.m_dirty = false;
        }

        ImGui::Render();
        if (rendering)
        {
            void* pixels;
            int   pitch;
            SDL_LockTexture(texture, nullptr, &pixels, &pitch);
            uint8_t* dst = static_cast<uint8_t*>(pixels);

            camera.render(&aggregate, render_target);
            render_target.frame++;

            for (int y = 0; y < settings.m_image_height; ++y)
            {
                for (int x = 0; x < settings.m_image_width; ++x)
                {
                    const vec3& c = render_target.render_target[x + y * settings.m_image_width];
                    uint8_t*    p = dst + y * pitch + x * 4;

                    // Assuming vec3 is linear float [0..1], apply gamma correction
                    p[0] = static_cast<uint8_t>(std::clamp(garbage::linear_to_gamma(c[0]), 0.0, 1.0) * 255.0); // R
                    p[1] = static_cast<uint8_t>(std::clamp(garbage::linear_to_gamma(c[1]), 0.0, 1.0) * 255.0); // G
                    p[2] = static_cast<uint8_t>(std::clamp(garbage::linear_to_gamma(c[2]), 0.0, 1.0) * 255.0); // B
                    p[3] = 255;                                                                                // A
                }
            }
            SDL_UnlockTexture(texture);
        }

        SDL_RenderTexture(renderer, texture, nullptr, nullptr);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }

    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    // TODO: Make collecting counters and stats more easy then ifdef
#ifndef NDEBUG
    fmt::println("{} primitive intersection calls ", aggregate.primitive_intersection_called);
    fmt::println("{} aabb intersection calls ", aggregate.aabb_test);

    fmt::println("Emitted {} primary rays", settings.m_image_height * settings.m_image_width * settings.m_samples_per_pixel);
    fmt::println("{} rays rejected by bounding box", aggregate.aabb_reject);
    fmt::println("{} rays missed", aggregate.missed);

#endif

    fmt::println("Writing to {}", destination.string());
    garbage::write_png(render_target.render_target.data(), render_target.width, render_target.height, destination);

    return 0;
}
