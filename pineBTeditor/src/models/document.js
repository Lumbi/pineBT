import { rootBehavior } from './behavior'

export function resetDocument(document) {
    document.setBehaviors([rootBehavior()])
    document.setConnections([])
}

export function updateBehavior(document, update) {
    const { behaviors, setBehaviors } = document
    setBehaviors(
        behaviors.map(behavior => {
            if (behavior.id === update.id) {
                return {...behavior, ...update}
            } else {
                return behavior
            }
        })
    )
}

export function updateBehaviors(document, behaviors) {
    const { setBehaviors } = document
    // TODO: Support partial updates instead of overwrite
    setBehaviors(behaviors)
}

export function deleteBehaviorById(document, behaviorId) {
    const { behaviors, setBehaviors } = document
    const { connections, setConnections } = document
    setConnections(
        connections.filter(c => c.from !== behaviorId && c.to !== behaviorId)
    )
    setBehaviors(
        behaviors.filter(b => b.id !== behaviorId)
    )
}

export function toDocumentData(document) {
    const { behaviors, connections } = document
    // TODO: Strip transient data like 'status'
    return JSON.stringify(
        {
            behaviors,
            connections,
        },
        undefined,
        4
    )
}
