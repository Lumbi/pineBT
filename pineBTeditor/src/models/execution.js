export function run(execution, blueprint) {
    const { 
        runningBehaviorTreeHandle,
     } = execution

    if (runningBehaviorTreeHandle) { 
        return
    }

    const handle = pineBT.create(JSON.stringify(blueprint))
    execution.setRunningBehaviorTreeHandle(handle)
    execution.setRunningBlueprint(blueprint)
    return handle
}

export function step(execution, handle) {
    const { document } = execution
    const { blackboard } = document

    // Synchronize blackboard with execution
    blackboard.entries.forEach(entry => {
        const { key, value } = entry
        if (isNaN(value)) {
            pineBT.blackboard.clear(handle, key)
        } else {
            pineBT.blackboard.set(handle, key, value)
        }
    })

    pineBT.run(handle)
    const statuses = pineBT.status(handle)
    execution.setStatuses(
        Object.fromEntries(
            statuses.map(_ => [_.id, _.status.toLowerCase()])
        )
    )
}

export function clearBlackboardKey(execution, key) {
    const { runningBehaviorTreeHandle } = execution
    pineBT.blackboard.clear(runningBehaviorTreeHandle, key)
}

export function stop(execution) {
    const { runningBehaviorTreeHandle } = execution
    if (runningBehaviorTreeHandle) {
        pineBT.destroy(runningBehaviorTreeHandle)
    }
    execution.setStatuses({})
    execution.setRunningBehaviorTreeHandle(null)
    execution.setRunningBlueprint(null)
}

export function statusForBehavior(execution, behavior) {
    if (!behavior) { return undefined }
    return execution.statuses[behavior.id]
}
