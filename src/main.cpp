//
// 簡易3D描画プロジェクト
//
#include <llmmetal/llmmetal.hpp>

namespace
{

//
//
//
class MyApp final : public llmmetal::AppHandler
{
public:
  void on_frame(llmmetal::Renderer &renderer, const llmmetal::InputState &input, double dt) override
  {
    renderer.clear({0.09f, 0.10f, 0.14f, 1.0f});

    const float          aspect = static_cast<float>(renderer.drawable_width()) /
                                  static_cast<float>(renderer.drawable_height());
    const llmmetal::Vec3 eye{0.0f, 2.5f, 8.0f};

    auto mdlView = llmmetal::Mat4::look_at(eye, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
    auto persMtx = llmmetal::Mat4::perspective(60.0f * 3.14159265f / 180.0f, aspect, 0.01f, 100.0f);

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
    renderer.draw_plane_3d({0, 0, 0}, {20, 20}, {0, 0.5f, 0.7f, 1.0f});

    renderer.draw_text_2d("Simple簡易描画", {40.0f, 24.0f}, 48.0f, {0.95f, 0.95f, 0.98f, 1.0f});
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
