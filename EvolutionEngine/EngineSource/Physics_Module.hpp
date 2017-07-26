
class Physics_Module : public Module
{
public:

    class Physics_Module_Factory : public Factory
    {
    public:

        Physics_Module_Factory()
        {
            Module::register_module ("physics", this);
        }

        shared_ptr< Module > create_module () override
        {
            return shared_ptr< Module >(new Physics_Module);
        }
    };


    class Physics_task : public Task
    {
        Physics_Module & module;

        Physics_task(Physics_Module & module) : module(module)
        {
        }

        bool initialize()
        {
            module.world.reset (new btDynamicWorld(...));
            // ...
        }

        bool finalize()
        {
            // ...
        }

        bool runstep(float time)
        {
            module.world->Step(time);
        }
    };

    class Physics_Component : public Component
    {
        btRight world;
    };

    static Physics_Module_Factory factory;
    // Definir Physics_Module_Factory Physics_Module::factory(); un archivo .CPP

    Physics_task task;

    shared_ptr< btDynamicWorld > world;

public:

    Physics_Module() : task(*this)
    {
    }

    Task * get_task() // Override
    {
        return task;
    }

};
