#ifndef ACTOR_HPP
#define ACTOR_HPP

#include "Component/Component/Component.hpp"
#include <vector>
#include <memory>
#include <typeinfo>
#include <algorithm>

/**
 * Actor class following the Component pattern from Game Programming Patterns.
 * An Actor is a container for components and implements the Update Method pattern.
 * It owns components and delegates behavior to them.
 */
class Actor {
public:
    Actor(float x = 0.0f, float y = 0.0f) : x_(x), y_(y), active_(true) {}
    
    virtual ~Actor() {
        cleanup();
    }

    /**
     * Update method called once per frame.
     * This implements the Update Method pattern by updating all active components.
     */
    void update(float deltaTime) {
        if (!active_) return;

        // Update all active components
        for (auto& component : components_) {
            if (component && component->isActive()) {
                component->update(*this, deltaTime);
            }
        }
    }

    /**
     * Add a component to this actor.
     * The actor takes ownership of the component.
     */
    template<typename T>
    T* addComponent(std::unique_ptr<T> component) {
        static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");
        
        T* componentPtr = component.get();
        components_.push_back(std::move(component));
        componentPtr->initialize();
        return componentPtr;
    }

    /**
     * Create and add a component to this actor.
     * Template arguments are forwarded to the component constructor.
     */
    template<typename T, typename... Args>
    T* addComponent(Args&&... args) {
        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        return addComponent(std::move(component));
    }

    /**
     * Get the first component of the specified type.
     * Returns nullptr if no component of that type is found.
     */
    template<typename T>
    T* getComponent() const {
        static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");
        
        for (const auto& component : components_) {
            T* typedComponent = dynamic_cast<T*>(component.get());
            if (typedComponent) {
                return typedComponent;
            }
        }
        return nullptr;
    }

    /**
     * Get all components of the specified type.
     */
    template<typename T>
    std::vector<T*> getComponents() const {
        static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");
        
        std::vector<T*> result;
        for (const auto& component : components_) {
            T* typedComponent = dynamic_cast<T*>(component.get());
            if (typedComponent) {
                result.push_back(typedComponent);
            }
        }
        return result;
    }

    /**
     * Remove a specific component from this actor.
     */
    template<typename T>
    bool removeComponent() {
        static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");
        
        auto it = std::find_if(components_.begin(), components_.end(),
            [](const std::unique_ptr<Component>& component) {
                return dynamic_cast<T*>(component.get()) != nullptr;
            });

        if (it != components_.end()) {
            (*it)->cleanup();
            components_.erase(it);
            return true;
        }
        return false;
    }

    /**
     * Remove all components from this actor.
     */
    void clearComponents() {
        for (auto& component : components_) {
            if (component) {
                component->cleanup();
            }
        }
        components_.clear();
    }

    // Position getters and setters
    float getX() const { return x_; }
    float getY() const { return y_; }
    void setX(float x) { x_ = x; }
    void setY(float y) { y_ = y; }
    void setPosition(float x, float y) { x_ = x; y_ = y; }

    // Active state
    bool isActive() const { return active_; }
    void setActive(bool active) { active_ = active; }

    // Component count
    size_t getComponentCount() const { return components_.size(); }

private:
    void cleanup() {
        clearComponents();
    }

    std::vector<std::unique_ptr<Component>> components_;
    float x_, y_;
    bool active_;
};

#endif // ACTOR_HPP