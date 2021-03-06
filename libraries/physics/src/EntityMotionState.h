//
//  EntityMotionState.h
//  libraries/entities/src
//
//  Created by Andrew Meadows on 2014.11.06
//  Copyright 2013 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#ifndef hifi_EntityMotionState_h
#define hifi_EntityMotionState_h

#include <AACube.h>

#include "ObjectMotionState.h"
#ifndef USE_BULLET_PHYSICS
// ObjectMotionState stubbery
class ObjectMotionState {
public:
    // so that this stub implementation is not completely empty we give the class a data member
    bool _stubData;
};
#endif // USE_BULLET_PHYSICS

class EntityItem;

// From the MotionState's perspective:
//      Inside = physics simulation
//      Outside = external agents (scripts, user interaction, other simulations)

class EntityMotionState : public ObjectMotionState {
public:

    // The OutgoingEntityQueue is a pointer to a QSet (owned by an EntitySimulation) of EntityItem*'s 
    // that have been changed by the physics simulation.
    // All ObjectMotionState's with outgoing changes put themselves on the list.
    static void setOutgoingEntityList(QSet<EntityItem*>* list);
    static void enqueueOutgoingEntity(EntityItem* entity);

    EntityMotionState(EntityItem* item);
    virtual ~EntityMotionState();

    /// \return MOTION_TYPE_DYNAMIC or MOTION_TYPE_STATIC based on params set in EntityItem
    MotionType computeMotionType() const;

#ifdef USE_BULLET_PHYSICS
    // this relays incoming position/rotation to the RigidBody
    void getWorldTransform(btTransform& worldTrans) const;

    // this relays outgoing position/rotation to the EntityItem
    void setWorldTransform(const btTransform& worldTrans);
#endif // USE_BULLET_PHYSICS

    // these relay incoming values to the RigidBody
    void applyVelocities() const;
    void applyGravity() const;

    void computeShapeInfo(ShapeInfo& info);

    void sendUpdate(OctreeEditPacketSender* packetSender, uint32_t frame);

    uint32_t getIncomingDirtyFlags() const { return _entity->getDirtyFlags(); }
    void clearIncomingDirtyFlags(uint32_t flags) { _entity->clearDirtyFlags(flags); }

protected:
    EntityItem* _entity;
};

#endif // hifi_EntityMotionState_h
