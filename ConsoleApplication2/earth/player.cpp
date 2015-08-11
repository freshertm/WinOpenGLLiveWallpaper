#include "player.h"
//#include "QDebug"
#include "glstarseffect.h"

#include "glmaterial.h"
#include "customsphere.h"
#include "sky/skymodel.h"
#include "glmodel.h"

#include <stdint.h>

const float realEarthRadius =  6371.0f; //km
const float realMoonDistance = 384403.0f; //km
const float realMoonRadius = 1738.0f;  //km
const float realAtmosphereWidth = 400.0f; //120.0f; //km
const float realSunDistance = 15000000.0f; //km

const float earthRadius = 1.0f;
const float skyRadius = earthRadius + realAtmosphereWidth * earthRadius / realEarthRadius;
const float skyAttInFactor = 25.0f;
const float skyAttOutFactor = 2.0f;

const float moonRadius = realMoonRadius / realEarthRadius;
const float moonDistance = realMoonDistance / realEarthRadius;

const float camOrbite = earthRadius * 3.0f;

Player::Player():_camOffset(camOrbite),_sunOffset(300.f), camPos(camOrbite, 0.0f, 0.0f),
    camLookVector(-1.f, 0.f, 0.f), camUpVector(0.f, 1.f, 0.f)
{
//    elTimer.start();
}

void Player::createScene(GLRender *render)
{   
    _render = render;

    GLMaterial mat;
    mat.setAmbient(Color4(0.0f, 0.0f, 0.0f, 1.f));
    mat.setDiffuse(Color4(1.1f, 1.1f, 1.3f));
    mat.setSpecular(Color4(0.2f, 0.2f, 0.3f));
    mat.setShininess(3);


    Bitmap * earthTexture = loadTexture(":/textures/earth");
    mat.setTexture(*earthTexture);
    delete earthTexture;

    CustomSphere * earth = new CustomSphere(earthRadius,80,80);
    _earth = earth;
    _earth->setMaterial(mat);
    _earth->position= vec4(0,0,0);
    _earth->rotate = vec4(-98.f, 0.f, 0.f);
    _scene.addRenderable(_earth);

    _sky = new SkyModel(skyRadius, earthRadius, 80);
    _sky->setSkyColor(Color4(0.6f, 0.6f, 0.8f, 1.0f));
    _sky->setAttInFactor(skyAttInFactor);
    _sky->setAttOutFactor(skyAttOutFactor);
    earth->addChild(_sky);


    GLMaterial moonMaterial;

    moonMaterial.setAmbient(Color4(0.01f, 0.01f, 0.01f));
    moonMaterial.setDiffuse(Color4(1.2f, 1.2f, 1.2f));
    moonMaterial.setSpecular(Color4(0.0f, 0.0f, 0.0f, 0.0f));
    moonMaterial.setShininess(10);

    Bitmap * moonTexture = loadTexture(":/textures/moon");
    moonMaterial.setTexture(*moonTexture);
    delete moonTexture;
    CustomSphere * moon = new CustomSphere(moonRadius, 10, 10);
    moon->setMaterial(moonMaterial);
    moon->position=vec3(realMoonDistance / realEarthRadius,0,0);
    _earth->addChild(moon);





    _stars = new GLStarsEffect(1000, vec4(0,0,0),  vec4(2000,2000,2000));
    _scene.addRenderable(_stars);

    _sun = new GLLight();
    _sun->specular(Color4(2, 2, 2));
    _sun->diffuse(Color4(1,1,1));
    _scene.addLight(_sun);
    _sun->setPosition(vec4(0, 0 , 0));

    _cam = new GLCamera();
    _cam->setFrustum(30, 0.005f, 1000);
    _cam->setPosition(vec3(0,0,-_camOffset));
    _cam->setLookPoint(vec3(0,0,0));
    _render->setCamera(_cam);

  /*  QGLShaderProgram * shader = new QGLShaderProgram();

    qDebug() << "vert: " << shader->addShaderFromSourceFile(QGLShader::Vertex, ":/shaders/earth.vert");
    qDebug() << "frag: " << shader->addShaderFromSourceFile(QGLShader::Fragment, ":/shaders/earth.frag");
    qDebug() << "link: " << shader->link();
    _earth->setShader(shader);

    moon->setShader(shader);*/
}

void Player::step()
{
    /*float camx = _earth->position.x + _camOffset * sin(_camRotateZ * 3.14 / 180.0) * cos(_camRotateX * 3.14 / 180.0);
    float camy = _earth->position.y + _camOffset * sin(_camRotateX * 3.14 / 180.0);
    float camz = _earth->position.z + _camOffset * cos(_camRotateZ * 3.14 / 180.0);*/



/*    int64_t msec = elTimer.elapsed();
    elTimer.start();*/

   // calcCameraPosition(msec);

    static float deg = 0.0;
    float ligx = _earth->position.x + _sunOffset * sin(deg * 3.14 / 180.0);
    float ligz = _earth->position.z + _sunOffset * cos(deg * 3.14 / 180.0);
    _sun->setPosition(vec3(ligx,0,ligz));
    deg += 0.01f;

}

void Player::key(int key)
{
    const float step = 0.1;
    const float rotateStep = 1;
   /* switch(key)
    {
    */
   /* case Qt::Key_Left:
        camLookVector = camLookVector.Rotate(DEGTORAD(-rotateStep), camUpVector);
        break;

    case Qt::Key_Right:
        camLookVector = camLookVector.Rotate(DEGTORAD(rotateStep), camUpVector);
        break;

    case Qt::Key_Up:
    {
        vec3 forward = camLookVector;
        forward.Normalize();
        camPos += forward * step;
    }
        break;

    case Qt::Key_Down:
    {
        vec3 forward = camLookVector;
        forward.Normalize();
        camPos -= forward * step;
    }
        break;

        */

    /*case Qt::Key_Minus:
        camPos = camPos  / 1.05f;
        break;

    case Qt::Key_Plus:
        camPos = camPos  * 1.05f;
        break;*/

   /* case Qt::Key_W:
    {
        vec3 v = _cam->lookPoint();
        v.z = v.z + step;
        _cam->setLookPoint(v);
    }
        break;

    case Qt::Key_S:
    {
        vec3 v = _cam->lookPoint();
        v.z = v.z - step;
        _cam->setLookPoint(v);
    }
        break;

    case Qt::Key_Q:
    {
        vec3 v = _cam->lookPoint();
        v.y = v.y + step;
        _cam->setLookPoint(v);
        break;
    }

    case Qt::Key_E:
    {
        vec3 v = _cam->lookPoint();
        v.y = v.y - step;
        _cam->setLookPoint(v);
        break;
    }


    case Qt::Key_A:
    {
        vec3 v = _cam->lookPoint();
        v.x = v.x + step;
        _cam->setLookPoint(v);
    }
        break;

    case Qt::Key_D:
    {
        vec3 v = _cam->lookPoint();
        v.x = v.x - step;
        _cam->setLookPoint(v);
    }
        break;

    case Qt::Key_R:
        _sunOffset += step;
        break;

    case Qt::Key_F:
        _sunOffset -= step;
        break;

    case Qt::Key_N:
    {
        vec4 r = _earth->rotate;
        r.x = r.x + 0.5;
        _earth->rotate = r;
        qDebug() << "r.x: " << r.x;
        break;
    }
    case Qt::Key_M:
    {
        vec4 r = _earth->rotate;
        r.x = r.x - 0.5;
        _earth->rotate = r;
        qDebug() << "r.x: " << r.x;
        break;
    }

    case Qt::Key_U:
        _sky->setAttInFactor(_sky->attInFactor() + 0.5);
        qDebug() << "sky In Factor" << _sky->attInFactor();
        break;

    case Qt::Key_J:
        _sky->setAttInFactor(_sky->attInFactor() - 0.5);
        qDebug() << "sky In Factor" << _sky->attInFactor();
        break;

    case Qt::Key_I:
        _sky->setAttOutFactor(_sky->attOutFactor() + 0.5);
        qDebug() << "sky Out Factor" << _sky->attOutFactor();
        break;

    case Qt::Key_K:
        _sky->setAttOutFactor(_sky->attOutFactor() - 0.5);
        qDebug() << "sky Out Factor" << _sky->attOutFactor();
        break;
    }*/
}

const GraphicScene &Player::scene()
{
    return _scene;
}

GLModel* Player::loadModel(const char *filename)
{
   /* SceneModel * obj = new SceneModel();

    qDebug() << "Loading models "<<endl;
    std::string res = tinyobj::LoadObj(obj->shapes,obj->materials,filename);
    qDebug() << res.c_str();
    qDebug() << "Loading models finished"<<endl;

    return _scene.addRenderable(*obj);*/
    return 0;
}

Bitmap *Player::loadTexture(const char *filename)
{
    /*QImage img(filename);
    QImage converted = img.convertToFormat(QImage::Format_RGB888);
    Bitmap *tex = new Bitmap;
    tex->width = converted.width();
    tex->height = converted.height();
    //QImage::Format fmt = converted.format();
    tex->format = Bitmap::RGB; //(fmt == QImage::Format_RGB888)? : Bitmap::RGBA;
    int64_t length = tex->width * tex->height * 3;
    tex->data = new unsigned char[length];
    memcpy(tex->data, converted.bits(), length);
    return tex;*/
    return nullptr;
}

const float camSpeed = 1; //градусов в сеукнду.
const float camLookForward=120.0f; //градусов

const float realMinCamHeight = 1000; //km
const float realMaxCamHeight = 3000; //km

const float minRadius = (realEarthRadius + realMinCamHeight) / realEarthRadius;
const float maxRadius = (realEarthRadius + realMaxCamHeight) / realEarthRadius;


void Player::calcCameraPosition(int64_t msec)
{
    _cam->setPosition(camPos);
    _cam->setLookPoint(camPos + camLookVector);
    _cam->setUp(camUpVector);
}
/*
void Player::calcCameraPosition(qint64 msec)
{
    float radians = DEGTORAD((camSpeed / 1000.0f) * ((float)(msec)));
    vec3 axis(0.0f, 1.0f, 0.0f);
    axis = axis.Rotate(DEGTORAD(25),vec3(1.0f, 0.0f, 0.0f));

    vec3 pos = _cam->position();

    if (pos.Magnitude() > maxRadius)
    {
        pos.Normalize();
        pos *= maxRadius;
    }

    if (pos.Magnitude() < minRadius)
    {
        pos.Normalize();
        pos *= minRadius;
    }

    _cam->setPosition( pos.Rotate(radians, axis));
    _cam->setLookPoint(pos.Rotate(radians + DEGTORAD(camLookForward), axis));

    pos.Normalize();
    _cam->setUp(-pos);

}*/
