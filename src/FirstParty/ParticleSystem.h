//
//  ParticleSystem.h
//  game_engine
//
//  Created by Jacob Robinson on 4/12/24.
//  Created for EECS 498: Game Engine Architecture at the University of Michigan
//
//  School Email: mrjacob@umich.edu

#ifndef ParticleSystem_h
#define ParticleSystem_h

#include <math.h>
#include <vector>
#include <queue>

#include "PhysicsWorld.h"
#include "Renderer.h"
#include "EngineUtils.h"
#include "ImageDB.h"

using namespace std;

class Actor;

struct Particle
{
    b2Body* body = nullptr;
    float age = 0.0f; // The number of seconds that this particle has been active for.
    float size = 1.0f; // The size of this particle
    float direction = 0.0f; // The direction that this particle was emitted in.
    float color[4] = {255.0f, 255.0f, 255.0f, 255.0f}; // The color of this particle in RGBA
    int former_color_index = 0; // The index of the color that this particle is transitioning away from.
    int future_color_index = 1; // The index of the color that this particle is transitioning to.
};

class ParticleSystem
{
public:
    int MAX_NUM_PARTICLES = 10000;
    int num_particles = 0;
    int framesActive = 0; // The number of frames that this system has been emitting for.
    std::queue<Particle*> particles;
    
    // Base component values
    std::string type = "Rigidbody";
    std::string key = "???";
    Actor* actor = nullptr;
    bool REMOVED_FROM_ACTOR = false;
    bool enabled = true;
    
    // Emitter parameters
    bool emitting = true;
    float duration = 5.0f; // How long the system emmits for before it loops/ends.
    bool loop = false; // Wether the emmiter runs again after the time is up.
    int emission_rate = 1; // The number of particles emited per second.
    float particle_lifetime = 5.0f; // The number of seconds that each particle is alive in the world for.
    float emitter_pos_x = 0.0f;
    float emitter_pos_y = 0.0f;
    float emission_range_x = 1.0f;
    float emission_range_y = 1.0f;
    int emission_direction = 0; // The direction that particles will be emmited in (degrees)
    int emission_direction_range = 180; // The range of the direction particles will be emmited in (counterclockwise from the emission direction)
    
    // Movement parameters
    bool change_movement = false;
    std::string movement_pattern = "none"; // none = no movement modifier, sine = sine wave movement, exp = exponential speed change
    float speed = 1.0f; // The speed at which particles are propeled when first emitted.
    float speed_sine_loop = 5.0f;
    float speed_sine_amplitude = 1.0f; // The maximum direction from start that the particles can go.
    
    // Size parameters
    bool change_size = false;
    std::string size_pattern = "linear"; // linear = linear growth, sine = sine wave of starting size, exp = exponential growth
    float size_change_per_second = 0.0f;
    float size_sine_loop = 5.0f; // The number of seconds that the sine pattern takes to loop
    
    // Rendering parameters
    std::string image = "";
    bool change_color = false;
    std::vector<std::vector<float>> colors = {{255.0f, 255.0f, 255.0f, 255.0f, 0.0f}}; // RGBA + Percent of the lifetime that this color is the full color of the particle.
    int sorting_order = 0;
    
    // All of these physics values are presets that will be applied to every single particle emmited from this system.
    // Rigidbody values
    float x = 0.0f;
    float y = 0.0f;
    std::string body_type = "dynamic";
    bool precise = true;
    float gravity_scale = 1.0f;
    float density = 1.0f;
    float angular_friction = 0.3f;
    float rotation = 0.0f;
    
    // Mass Data
    float mass = 0.01f; // The mass of a single particle (in Kilograms)
    
    // Collider
    bool has_collider = true;
    std::string collider_type = "box";
    float starting_size = 1.0f;
    float friction = 0.3f;
    float bounciness = 0.3f;
    
    // TODO: Particles shouldn't trigger the OnCollision or OnTriggerEnter of any rigidbody.
    b2BodyDef particleBodyDef;
    b2FixtureDef fixtureDef;
    b2MassData massData;
    
    // Chaches the fixtures at a bunch of different sizes so that we don't have to keep making new ones and leaking memory by making new shapes.
    std::unordered_map<int, b2FixtureDef*> colliders_by_size;
    
    // Particle System functions
    void StartEmitting();
    void StopEmitting();
    void CreateParticle();
    void UpdateParticle(Particle* particle);
    std::vector<float> GetDeltaColor(Particle* particle);
    b2FixtureDef* GetNewCollider(float size);
    void DestroyParticle(Particle* particle);
    void RenderParticle(Particle* particle);
    
    // Standard lifecycle functions
    void OnStart();
    void OnUpdate();
    void OnDestroy();
};

// Enforces colors being sorted by their given percentage
struct ColorSorterComparator
{
    // Comparator function
    bool operator()(std::vector<float> A, std::vector<float> B)
    {
        if (A[4] == B[4])
        {
            return 0;
        }
        
        return A[4] < B[4];
    }
};

#endif /* ParticleSystem_h */
