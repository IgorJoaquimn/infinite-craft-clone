#ifndef COMPONENT_HPP
#define COMPONENT_HPP

// Forward declaration
class Actor;

/**
 * Base Component class following the Component pattern from Game Programming Patterns.
 * All game components inherit from this class and implement their specific update logic.
 */
class Component {
public:
    Component() = default;
    virtual ~Component() = default;

    /**
     * Update method called once per frame for each component.
     * This implements the Update Method pattern.
     * 
     * @param actor Reference to the actor that owns this component
     * @param deltaTime Time elapsed since last frame (in seconds)
     */
    virtual void update(Actor& actor, float deltaTime) = 0;

    /**
     * Initialize the component when it's added to an actor.
     * Called once when the component is attached.
     */
    virtual void initialize() {}

    /**
     * Cleanup when the component is removed or destroyed.
     */
    virtual void cleanup() {}

    /**
     * Check if this component is active and should be updated.
     */
    virtual bool isActive() const { return active_; }

    /**
     * Set the active state of this component.
     */
    void setActive(bool active) { active_ = active; }

private:
    bool active_ = true;
};

#endif // COMPONENT_HPP