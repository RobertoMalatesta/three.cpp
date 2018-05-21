//
// Created by byter on 20.10.17.
//

#include "Background.h"
#include "Renderer_impl.h"

namespace three  {
namespace gl {

void Background::render(RenderList *renderList, const Scene::Ptr scene, const Camera::Ptr camera, bool forceClear)
{
  if(scene->hasBackground()) {
    if(three::Background<Color> *bg = scene->background()) {

      setClear( bg->data, 1 );
      renderer.clear( renderer.autoClearColor, renderer.autoClearDepth, renderer.autoClearStencil );
    }
    else if(three::Background<Texture::Ptr> *bg = scene->background()) {

      if ( renderer.autoClear || forceClear ) {
        renderer.clear( renderer.autoClearColor, renderer.autoClearDepth, renderer.autoClearStencil );
      }

      if(ImageCubeTexture *ict = bg->data->typer) {

        if (!boxMesh) {

          //TODO: find out why we have to pass 100000 rather than 1 as in original code
          geometry::buffer::Box::Ptr box = geometry::buffer::Box::make(100000, 100000, 100000);
          ShaderInfo si = shaderlib::get(ShaderID::cube);
          ShaderMaterial::Ptr sm = ShaderMaterial::make(
             si.uniforms, si.vertexShader, si.fragmentShader, Side::Back, true, false, false);

          boxMesh = MeshT<geometry::buffer::Box, ShaderMaterial>::make("background", box, sm);

          box->setNormal(nullptr);
          box->setUV(nullptr);

          boxMesh->onBeforeRender.connect([&] (Renderer &renderer, ScenePtr scene, CameraPtr camera,
                                               Geometry::Ptr geometry, Material::Ptr material,
                                               const Group *group) {
            boxMesh->matrixWorld().setPosition( camera->matrixWorld() );
          });

          geometries.update(box);
        }

        boxMesh->material<0>()->uniforms.set(UniformName::tCube, CAST2(bg->data, CubeTexture));

        renderList->push_front(boxMesh, boxMesh->geometry_t(), boxMesh->material(), 0, nullptr);
      }
      else if(ImageTexture *ict = bg->data->typer) {

        if (!planeCamera) {

          planeCamera = OrthographicCamera::make( -1, 1, 1, -1, 0, 1 );

          MeshBasicMaterial::Ptr mat = MeshBasicMaterial::make();
          mat->depthTest = false;
          mat->depthWrite = false;
          mat->fog = false;

          auto planeGeom = geometry::buffer::Plane::make( 2, 2 );
          planeMesh = MeshT<geometry::buffer::Plane, MeshBasicMaterial>::make(planeGeom, mat);

          geometries.update(planeGeom);
        }

        planeMesh->material<0>()->map = bg->data;

        // TODO Push this to renderList
        renderer.renderBufferDirect( planeCamera, nullptr, planeMesh->geometry_t(), planeMesh->material(), planeMesh, nullptr);
      }
    }
  }
  else {
    setClear( clearColor, clearAlpha );
    if ( renderer.autoClear || forceClear ) {
      renderer.clear( renderer.autoClearColor, renderer.autoClearDepth, renderer.autoClearStencil );
    }
  }
}

}
}

