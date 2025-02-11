//
//  GLGameScene.cpp
//  Geometry Lab
//
//  This is the primary class file for running the game.  You should study this 
//  file for ideas on how to structure your own root class. This class 
//  illustrates how to use the geometry tools in CUGL, and how to combine
//  them with the physics engine.
//
//  Note that this time we do not have any additional model classes.
//
//  Author: Walker White
//  Version: 1/29/24
//
#include <cugl/cugl.h>
#include <iostream>
#include <sstream>

#include "GLGameScene.h"

using namespace cugl;
using namespace std;

#pragma mark -
#pragma mark Level Layout

// Lock the screen size to fixed height regardless of aspect ratio
#define SCENE_HEIGHT 720

/** How big the handle should be */
#define KNOB_RADIUS 15

/** The extrustion width. */
#define LINE_WIDTH 50

/** The width of a handle. */
#define HANDLE_WIDTH 3

/** The ratio between the physics world and the screen. */
#define PHYSICS_SCALE 50

/** The initial control points for the spline. */
float CIRCLE[] = {    0,  200,  120,  200,
        200,  120,  200,    0,  200, -120,
        120, -200,    0, -200, -120, -200,
       -200, -120, -200,    0, -200,  120,
       -120,  200,    0,  200};


/** The (CLOCKWISE) polygon for the star */
float STAR[] = {     0,    50,  10.75,    17,   47,     17,
                 17.88, -4.88,   29.5, -40.5,    0, -18.33,
                 -29.5, -40.5, -17.88, -4.88,  -47,     17,
                -10.75,    17};

#pragma mark -
#pragma mark Constructors
/**
 * Initializes the controller contents, and starts the game
 *
 * The constructor does not allocate any objects or memory.  This allows
 * us to have a non-pointer reference to this controller, reducing our
 * memory allocation.  Instead, allocation happens in this method.
 *
 * @param assets    The (loaded) assets for this game mode
 *
 * @return true if the controller is initialized properly, false otherwise.
 */
bool GameScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
    // Initialize the scene to a locked width
    if (assets == nullptr) {
        return false;
    } else if (!Scene2::initWithHint(Size(0,SCENE_HEIGHT))) {
        return false;
    }
    
    // Start up the input handler
    _input.init();
    
    // ADD CODE HERE
    
    // Declaring Spline
    _spline.set(reinterpret_cast<Vec2*>(CIRCLE), 13);
    _spline.setClosed(true);
    
//    Star
    
    
    buildGeometry();
    
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void GameScene::dispose() {
    // NOTHING TO DO THIS TIME
}


#pragma mark -
#pragma mark Gameplay Handling
/**
 * The method called to indicate the start of a deterministic loop.
 *
 * This method is used instead of {@link #update} if {@link #setDeterministic}
 * is set to true. It marks the beginning of the core application loop,
 * which is concluded with a call to {@link #postUpdate}.
 *
 * This method should be used to process any events that happen early in
 * the application loop, such as user input or events created by the
 * {@link schedule} method. In particular, no new user input will be
 * recorded between the time this method is called and {@link #postUpdate}
 * is invoked.
 *
 * Note that the time passed as a parameter is the time measured from the
 * start of the previous frame to the start of the current frame. It is
 * measured before any input or callbacks are processed. It agrees with
 * the value sent to {@link #postUpdate} this animation frame.
 *
 * @param dt    The amount of time (in seconds) since the last frame
 */
void GameScene::preUpdate(float dt) {
    // We always need to call this to update the state of input variables
    // This SYNCHRONIZES the call back functions with the animation frame.
    _input.update();
    
    cugl::Vec2 screenMousePos = _input.getPosition();
    
    cugl::Vec2 worldMousePos = screenToWorldCoords(screenMousePos);
    worldMousePos -= getSize()/2;
    
    if (_input.didPress()) {
            int n = _spline.size();
            for (int i = 0; i < n; i++) {
                cugl::Vec2 leftHandle, rightHandle;

                if (i == 0) {
                    leftHandle = _spline.getTangent(2 * n - 1);
                } else {
                    leftHandle = _spline.getTangent(2 * i - 1);
                }
                rightHandle = _spline.getTangent(2 * i);

                // Check if the mouse clicked a knob
                if (leftHandle.distance(worldMousePos) < KNOB_RADIUS) {
                   
                    _selectedKnob = leftHandle;
            
                    break;
                } else if (rightHandle.distance(worldMousePos) < KNOB_RADIUS) {
                    _selectedKnob = rightHandle;
        
                    break;
                }
            }
        }
    
    
    
    
    
    // ADD CODE HERE
}

/**
 * The method called to provide a deterministic application loop.
 *
 * This method provides an application loop that runs at a guaranteed fixed
 * timestep. This method is (logically) invoked every {@link getFixedStep}
 * microseconds. By that we mean if the method {@link draw} is called at
 * time T, then this method is guaranteed to have been called exactly
 * floor(T/s) times this session, where s is the fixed time step.
 *
 * This method is always invoked in-between a call to {@link #preUpdate}
 * and {@link #postUpdate}. However, to guarantee determinism, it is
 * possible that this method is called multiple times between those two
 * calls. Depending on the value of {@link #getFixedStep}, it can also
 * (periodically) be called zero times, particularly if {@link #getFPS}
 * is much faster.
 *
 * As such, this method should only be used for portions of the application
 * that must be deterministic, such as the physics simulation. It should
 * not be used to process user input (as no user input is recorded between
 * {@link #preUpdate} and {@link #postUpdate}) or to animate models.
 *
 * The time passed to this method is NOT the same as the one passed to
 * {@link #preUpdate}. It will always be exactly the same value.
 *
 * @param step  The number of fixed seconds for this step
 */
void GameScene::fixedUpdate(float step) {
    // ADD CODE HERE
}

/**
 * The method called to indicate the end of a deterministic loop.
 *
 * This method is used instead of {@link #update} if {@link #setDeterministic}
 * is set to true. It marks the end of the core application loop, which was
 * begun with a call to {@link #preUpdate}.
 *
 * This method is the final portion of the update loop called before any
 * drawing occurs. As such, it should be used to implement any final
 * animation in response to the simulation provided by {@link #fixedUpdate}.
 * In particular, it should be used to interpolate any visual differences
 * between the the simulation timestep and the FPS.
 *
 * This method should not be used to process user input, as no new input
 * will have been recorded since {@link #preUpdate} was called.
 *
 * Note that the time passed as a parameter is the time measured from the
 * last call to {@link #fixedUpdate}. That is because this method is used
 * to interpolate object position for animation.
 *
 * @param remain    The amount of time (in seconds) last fixedUpdate
 */
void GameScene::postUpdate(float remain) {
    // ADD CODE HERE
}

/**
 * Draws all this scene to its associated SpriteBatch.
 *
 * The default implementation of this method simply draws the scene graph
 * to the sprite batch. By overriding it, you can do custom drawing
 * in its place. The expectation is that you will use the associated
 * SpriteBatch for drawing, but this is not required.
 */
void GameScene::render() {
    
    // DO NOT DO THIS IN YOUR FINAL GAME
    _batch->begin(getCamera()->getCombined());
    
    cugl::Vec2 offset = getSize() / 2;
    
    _batch -> setColor(cugl::Color4::BLACK);
    _batch-> outline(_flattenedPath, (getSize() / 2));
    _batch-> fill(_extrudedSpline, (getSize()/2));
    
    for (const auto& handle : containerHandles) {
            _batch -> setColor(cugl::Color4::WHITE);
            _batch-> fill(handle, offset);
    }
    
    for (const auto& knob : containerKnobs) {
        _batch -> setColor( cugl::Color4::RED);
        _batch->fill(knob, offset);
    }
    // Draw the star in blue at the center
    _batch->setColor(cugl::Color4::BLUE);
    _batch->fill(_triangulatedStar, offset);
    
    _batch -> end();
}

/**
 * Rebuilds the geometry.
 *
 * This method should recreate all the polygons for the spline, the handles
 * and the falling star. It should also recreate all physics objects.
 *
 * However, to cut down on performance overhead, this method should NOT add
 * those physics objects to the world inside this method (as this method is
 * called repeatedly while the user moves a handle). Instead, those objects
 * should not be activated until the state is "stable".
 */
void GameScene::buildGeometry() {
    
//    Flatten Spline
    cugl::SplinePather pather;
    pather.set(&_spline);
    pather.calculate();
    _flattenedPath = pather.getPath();
    
//    Extrude Flattened Spline
    cugl::SimpleExtruder extruder;
    extruder.set(_flattenedPath);
    extruder.calculate(LINE_WIDTH);
    _extrudedSpline = extruder.getPolygon();

    

    
// Control Handles
    int n = _spline.size();
    for (int i = 0; i < n; i++){
        cugl::Vec2 leftHandle;
        
        if (i== 0){
             leftHandle = _spline.getTangent( 2* n -1);
        }
        else {
            leftHandle = _spline.getTangent( 2* i -1);
        }
        
        cugl::Vec2 rightHandle =  _spline.getTangent(2*i);
        
        cugl::Spline2 _line;
        _line.set(leftHandle, rightHandle);
        
        
        cugl::SplinePather pather_temp;
        cugl::Path2 _path;
        pather_temp.set(&_line);
        pather_temp.calculate();
        _path = pather_temp.getPath();
        
        cugl::SimpleExtruder extruder_handle;
        cugl::Poly2 _extrudedHandle;
        
        extruder_handle.set(_path);
        extruder_handle.calculate(HANDLE_WIDTH);
        _extrudedHandle = extruder_handle.getPolygon();
        
        containerHandles.push_back(_extrudedHandle);
        
        cugl::PolyFactory polyfac;
        cugl::Poly2 _left_knob = polyfac.makeCircle(leftHandle, KNOB_RADIUS);
        cugl::Poly2 _right_knob = polyfac.makeCircle(rightHandle, KNOB_RADIUS);
        
        containerKnobs.push_back(_left_knob);
        containerKnobs.push_back(_right_knob);

    }
// Star
    cugl::Path2 star_path;
    star_path.set(reinterpret_cast<Vec2*>(STAR), 10);
    star_path.reverse();
    
    cugl::EarclipTriangulator triangulator;
    triangulator.set(star_path);
    triangulator.calculate();
    _triangulatedStar = triangulator.getPolygon();
    
    
    
    
    
    

    


}
