//
//  ParticleSystem.cpp
//  game_engine
//
//  Created by Jacob Robinson on 4/12/24.
//  Created for EECS 498: Game Engine Architecture at the University of Michigan
//
//  School Email: mrjacob@umich.edu

#include <stdio.h>

#include "ParticleSystem.h"

// Particle System functions
void ParticleSystem::StartEmitting()
{
    emitting = true;
    framesActive = 0.0f;
}

void ParticleSystem::StopEmitting()
{
    emitting = false;
}

void ParticleSystem::CreateParticle()
{
    num_particles++;
    Particle* newParticle = new Particle();
    
    newParticle->size = starting_size;
    newParticle->color[0] = colors[0][0];
    newParticle->color[1] = colors[0][1];
    newParticle->color[2] = colors[0][2];
    newParticle->color[3] = colors[0][3];


    b2BodyDef bd = particleBodyDef;
    b2FixtureDef fd = fixtureDef;
    
    // Randomly assign this particles starting position in the range given
    bd.position.x = emitter_pos_x + EngineUtils::RandomNumber(-emission_range_x, emission_range_x, 1000);
    bd.position.y = emitter_pos_y + EngineUtils::RandomNumber(-emission_range_y, emission_range_y, 1000);
    
    newParticle->body = PhysicsWorld::world->CreateBody(&bd);
    newParticle->body->CreateFixture(&fd);
    newParticle->age = 0.0f;
    
    // Changes the mass of the particle to be uniform regardless of size
    b2MassData md = massData;
    newParticle->body->SetMassData(&md);
    
    // Applies a force in a random direction within the given range
    float direction = -emission_direction + EngineUtils::RandomNumber(-emission_direction_range / 2.0f, emission_direction_range / 2.0f, 1000);
    newParticle->direction = direction;
    direction *= (b2_pi / 180.0f);
    
    b2Vec2 velocity = b2Vec2(cos(direction) * speed, sin(direction) * speed);
    newParticle->body->SetLinearVelocity(velocity);
    
    particles.push(newParticle);
}

void ParticleSystem::UpdateParticle(Particle* particle)
{
    // Update particles according to a pattern (also update particle's hitbox size to match if size changes)
    
    // Update color
    if (change_color)
    {
        // The change in color per frame
        std::vector<float> delta_color = GetDeltaColor(particle);
        
        particle->color[0] += delta_color[0];
        if (particle->color[0] >= 255.0f) {particle->color[0] = 255.0f;}
        if (particle->color[0] <= 0.0f) {particle->color[0] = 0.0f;}
        
        particle->color[1] += delta_color[1];
        if (particle->color[1] >= 255.0f) {particle->color[1] = 255.0f;}
        if (particle->color[1] <= 0.0f) {particle->color[1] = 0.0f;}
        
        particle->color[2] += delta_color[2];
        if (particle->color[2] >= 255.0f) {particle->color[2] = 255.0f;}
        if (particle->color[2] <= 0.0f) {particle->color[2] = 0.0f;}
        
        particle->color[3] += delta_color[3];
        if (particle->color[3] >= 255.0f) {particle->color[3] = 255.0f;}
        if (particle->color[3] <= 0.0f) {particle->color[3] = 0.0f;}
    }
    
    // Update size
    if (change_size)
    {
        float delta_size = 0.0f;
        
        // Alters the size based on the pattern chosen
        switch (size_pattern[0])
        {
            case 'l':
                // Linear pattern
                delta_size = size_change_per_second / 60;
                particle->size += delta_size;
                break;
                
            case 'e':
                // Exponential pattern
                delta_size = std::powf(particle->age / particle_lifetime, 3.0f);
                particle->size += delta_size * size_change_per_second;
                break;
                
            case 's':
                // Sine pattern
                delta_size = std::sinf((particle->age / size_sine_loop) * (2.0f * b2_pi));
                particle->size = std::abs(delta_size * starting_size);
                break;
                
            default:
                break;
        }

        // Prevents particle from overflowing
        if (particle->size < 0.0f)
        {
            particle->size = -0.001f;
        }
        
        // Update hitbox to match the new size
        if (has_collider)
        {
            particle->body->DestroyFixture(particle->body->GetFixtureList());
            particle->body->CreateFixture(GetNewCollider(particle->size));
        }
    }
    
    // Update movement
    if (change_movement)
    {
        b2Vec2 velocity = b2Vec2(cos(particle->direction * (b2_pi / 180.0f)) * speed, sin(particle->direction * (b2_pi / 180.0f)) * speed);
        b2Vec2 newVelocity = velocity;
        float delta_speed;
        
        // Alters the movement based on the pattern chosen
        switch (movement_pattern[0]) 
        {
            case 's':
                // Sine pattern
                delta_speed = std::sinf((particle->age / speed_sine_loop) * (2.0f * b2_pi)) * speed_sine_amplitude;
                
                velocity = b2Vec2(-sin(particle->direction * (b2_pi / 180.0f)) * speed, cos(particle->direction * (b2_pi / 180.0f)) * speed);
                velocity.Normalize();
                velocity *= delta_speed;
                
                newVelocity += velocity;
                break;
                
            case 'e':
                break;
                
            default:
                break;
        }
        
        particle->body->SetLinearVelocity(newVelocity);
    }
}

std::vector<float> ParticleSystem::GetDeltaColor(Particle* particle)
{
    float percent_of_lifetime = particle->age / particle_lifetime;
    
    // Find the future and former colors
    if (colors.size() > 1)
    {
        if (percent_of_lifetime > colors[particle->future_color_index][4] / 100)
        {
            if (particle->future_color_index + 1 < colors.size())
            {
                particle->future_color_index++;
            }
            particle->former_color_index++;
            if (particle->former_color_index == particle->future_color_index)
            {
                particle->former_color_index--;
            }
        }
    }
    else
    {
        return {0, 0, 0, 0};
    }

    
    // The time that it takes to transition from the old color to the new one in seconds:
    float transition_time = (std::fabs(colors[particle->future_color_index][4] - colors[particle->former_color_index][4]) / 100) * particle_lifetime;
    
    // Calculates the change in the RGB and A values.
    std::vector<float> delta_color;
    for (int i = 0; i < 4; i++)
    {
        delta_color.push_back((colors[particle->future_color_index][i] - colors[particle->former_color_index][i]) / (60 * transition_time));
    }
    return delta_color;
}

b2FixtureDef* ParticleSystem::GetNewCollider(float size)
{
    if (colliders_by_size.find((int)(size * 1000)) == colliders_by_size.end())
    {
        b2FixtureDef* newFixture = new b2FixtureDef(fixtureDef);
        
        if (collider_type == "box")
        {
            b2PolygonShape* box = new b2PolygonShape;
            box->SetAsBox(size, size);
            newFixture->shape = box;
        }
        else if (collider_type == "circle")
        {
            b2CircleShape* circle = new b2CircleShape;
            circle->m_radius = size / 2;
            newFixture->shape = circle;
        }
        
        colliders_by_size[(int)(size * 1000)] = newFixture;
        
        return newFixture;
    }
    
    return colliders_by_size[size];
}

void ParticleSystem::DestroyParticle(Particle* particle)
{
    num_particles--;
    PhysicsWorld::world->DestroyBody(particle->body);
    delete particle;
}

void ParticleSystem::RenderParticle(Particle* particle)
{
    b2Vec2 position = particle->body->GetPosition();
    float rotation = particle->body->GetAngle() * (180 / b2_pi);
    
    // Ensures particles have a constanst size, not dependant on their sprite size
    int particleWidth = 0;
    int particleHeight = 0;
    SDL_QueryTexture(GetImage(image), NULL, NULL, &particleWidth, &particleHeight);
    float particleScale = particle->size / (particleWidth / RendererData::PIXELS_PER_METER);
    
    RendererData::DrawImageEx(image, position.x, position.y, rotation, particleScale, particleScale, 0.5f, 0.5f, particle->color[0], particle->color[1], particle->color[2], particle->color[3], sorting_order);
}

// Standard lifecycle functions
void ParticleSystem::OnStart()
{
    PhysicsWorld::InitializeWorld();
    
    // Sorts the colors by their percentages
    if (change_color && colors.size() > 1)
    {
        ColorSorterComparator comparator;
        std::sort(colors.begin(), colors.end(), comparator);
    }
    
    // Sets the mass data
    massData.mass = mass;

    // Creates the body template to use for particles:
    // Sets the body type
    switch (body_type[0])
    {
        case 'd':
            particleBodyDef.type = b2_dynamicBody;
            break;
            
        case 's':
            particleBodyDef.type = b2_staticBody;
            break;
            
        case 'k':
            particleBodyDef.type = b2_kinematicBody;
            break;
            
        default:
            break;
    }
    // Physics settings
    particleBodyDef.bullet = precise;
    particleBodyDef.angularDamping = angular_friction;
    particleBodyDef.gravityScale = gravity_scale;
    // Transform settings
    particleBodyDef.position.x = emitter_pos_x;
    particleBodyDef.position.y = emitter_pos_y;
    particleBodyDef.angle = rotation * (b2_pi / 180);
    
    // Gives the particles shape in the world
    
    /* phantom sensor to make particles move if a collider is not present */
    if (!has_collider)
    {
        // Determines what particles can collide with
        fixtureDef.filter.categoryBits = 0x0000;
        fixtureDef.filter.maskBits = 0x0000;
        
        b2CircleShape* phantom_shape = new b2CircleShape;
        phantom_shape->m_radius = starting_size / 2;
        
        fixtureDef.shape = phantom_shape;
        
        // Because it is a sensor (with no callback even), no collisions will ever occur
        fixtureDef.isSensor = true;
        fixtureDef.density = density;
    }
    // Creates a collider
    else
    {
        // Determines what particles can collide with
        fixtureDef.filter.categoryBits = 0x0004;
        fixtureDef.filter.maskBits = 0x0001;
        
        if (collider_type == "box")
        {
            b2PolygonShape* box = new b2PolygonShape;
            box->SetAsBox(starting_size, starting_size);
            fixtureDef.shape = box;
        }
        else if (collider_type == "circle")
        {
            b2CircleShape* circle = new b2CircleShape;
            circle->m_radius = starting_size / 2;
            fixtureDef.shape = circle;
        }
    
        fixtureDef.isSensor = false;
        fixtureDef.density = density;
        fixtureDef.friction = friction;
        fixtureDef.restitution = bounciness;
    }
    
    colliders_by_size[starting_size] = &fixtureDef;
}

void ParticleSystem::OnUpdate()
{
    /* Emission Phase */
    if (emitting && num_particles < MAX_NUM_PARTICLES)
    {
        // Emission rate is impercise with larger rates,
        // But the larger rates should result in a more accurate aproximation
        if (emission_rate > 60)
        {
            int num_particles_to_spawn = std::ceil(EngineUtils::RandomNumber(0, (emission_rate / 60.0f) * 2u, 100));
            //std::cout << num_particles_to_spawn << std::endl;
            if (num_particles + num_particles_to_spawn < MAX_NUM_PARTICLES)
            {
                for (int i = 0; i < num_particles_to_spawn; i++)
                {
                    CreateParticle();
                }
            }
        }
        // Emission rate is exact when less than 60, this is because each second is exactly 60 frames long.
        else if (framesActive % (60 / emission_rate) == 0)
        {
            CreateParticle();
        }
    }
    if (num_particles == 0) {return;}
    
    std::queue<Particle*> renderingQueue;
    
    /* Simulation Phase */
    // Physics done automatically by box2d
    while (!particles.empty())
    {
        Particle* particle = particles.front();
        particles.pop();
        
        UpdateParticle(particle);
        
        // Ages the particle by 1 frame
        particle->age += (1.0f / 60.0f);
        
        // Removes a particle from the queue if it's lifetime is up
        if (particle->age < particle_lifetime && particle->size >= 0.0f)
        {
            renderingQueue.push(particle);
        }
        else
        {
            DestroyParticle(particle);
        }
    }
    
    /* Rendering Phase */
    while (!renderingQueue.empty())
    {
        Particle* particle = renderingQueue.front();
        renderingQueue.pop();
        particles.push(particle);
        
        RenderParticle(particle);
    }
    
    // If this particle system has been emitting for longer than its duration then stop emitting.
    if (!loop && framesActive / 60.0f > duration)
    {
        emitting = false;
    }
    else
    {
        framesActive++;
    }
}

void ParticleSystem::OnDestroy()
{
    if (num_particles == 0) {return;}
    for (int i = 0; i < num_particles; i++)
    {
        Particle* particle = particles.front();
        particles.pop();
        DestroyParticle(particle);
    }
}
