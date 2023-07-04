const ROOT_BEHAVIOR_ID = 0

export function rootBehavior() {
    return {
        schema: 'Root',
        id: ROOT_BEHAVIOR_ID,
        position: { x: 400, y: 100 }
    }
}

export function isRoot(behavior) {
    return behavior.id === ROOT_BEHAVIOR_ID
}

export function updatedBehaviorWithOption(behavior, option) {
    return {
        ...behavior,
        options: {
            ...behavior.options,
            [option.key]: option.value
        }
    }
}
