//
// 簡易3D描画プロジェクト
//
#include <algorithm>
#include <cmath>
#include <llmmetal/llmmetal.hpp>

namespace
{

//
//
//
class MyApp final : public llmmetal::AppHandler
{
  using Texture = llmmetal::TextureHandle;
  Texture     water_{};
  std::string error_{};
  float       camera_yaw_   = 0.0f;
  float       camera_pitch_ = 0.30288488f;

public:
  void on_start(llmmetal::Renderer &renderer) override
  {
    llmmetal::TextureLoadOptions options;
    options.srgb             = true;
    options.flip_y           = false;
    options.generate_mipmaps = true;
    const auto loaded        = renderer.try_create_texture_from_file(
        std::filesystem::path("resources") / "WATER.png", options);
    water_ = loaded.texture;
    error_ = loaded.error_message;
    if (!water_)
    {
      water_ = renderer.create_checkerboard_texture(256, 256, 32);
    }
  }

  void on_frame(llmmetal::Renderer &renderer, const llmmetal::InputState &input, double dt) override
  {
    renderer.clear({0.09f, 0.5f, 0.75f, 1.0f});

    const float     aspect         = static_cast<float>(renderer.drawable_width()) /
                                     static_cast<float>(renderer.drawable_height());
    constexpr float pi             = 3.14159265f;
    constexpr float camera_radius  = 8.381527f;
    constexpr float orbit_speed    = 1.8f;
    constexpr float deadzone       = 0.12f;
    const auto      apply_deadzone = [](float value)
    { return std::abs(value) < deadzone ? 0.0f : value; };
    const float left_x = apply_deadzone(input.gamepad.left_stick_x);
    const float left_y = apply_deadzone(input.gamepad.left_stick_y);

    camera_yaw_ += left_x * orbit_speed * static_cast<float>(dt);
    camera_pitch_ += -left_y * orbit_speed * static_cast<float>(dt);
    camera_pitch_ = std::clamp(camera_pitch_, -0.45f * pi, 0.45f * pi);

    const float          pitch_cos = std::cos(camera_pitch_);
    const llmmetal::Vec3 eye{camera_radius * pitch_cos * std::sin(camera_yaw_),
                             camera_radius * std::sin(camera_pitch_),
                             camera_radius * pitch_cos * std::cos(camera_yaw_)};

    auto mdlView = llmmetal::Mat4::look_at(eye, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
    auto persMtx = llmmetal::Mat4::perspective(60.0f * pi / 180.0f, aspect, 0.01f, 100.0f);

    renderer.set_camera(mdlView, persMtx);
    renderer.set_camera_position(eye);
    renderer.set_light_direction({0.0f, -1.0f, 1.0f});
    renderer.set_ambient_light(0.3f);
    renderer.set_specular_strength(0.4f);
    renderer.set_shininess(36.0f);

    auto rotY   = -input.gamepad.right_stick_x;
    auto rotX   = -input.gamepad.right_stick_y;
    auto rotS   = 0.5f * 3.1415926f;
    auto rotRad = llmmetal::Vec3{rotX * rotS, rotY * rotS, 0.0f};

    renderer.draw_cube_3d({-0.7f, 1.0f, 1.0f}, {1, 1, 1}, rotRad, {0.95f, 0.55f, 0.25f, 1.0f});
    renderer.draw_plane_3d({0, 0, 0}, {20, 20}, {1.0f, 1.5f, 2.0f, 1.0f}, water_);
    renderer.draw_sphere_3d({0.0f, 0.0f, 0.0f}, 1.0f, 10, 5, {0.9f, 0.1f, 0.2f, 0.5f});

    renderer.draw_text_2d("Simple簡易描画", {40.0f, 24.0f}, 48.0f, {0.95f, 0.95f, 0.98f, 1.0f});
    if (!error_.empty())
    {
      renderer.draw_text_2d(error_, {40.0f, 100.0f}, 48.0f, {0.95f, 0.95f, 0.98f, 1.0f});
    }
  }
};

} // namespace

int
main(int argc, char **argv)
{
  MyApp app;

  llmmetal::AppConfig config;
  config.title = "Simple 3D";
  return run(config, app);
}
