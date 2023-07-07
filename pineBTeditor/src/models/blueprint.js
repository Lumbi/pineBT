export function from(behaviors, connections) {
    const entryConnection = connections.find(c => c.from === 0)
    if (!entryConnection) return undefined
    const entryBehavior= behaviors.find(b => b.id === entryConnection.to)
    if (!entryBehavior) return undefined

    function recursively(behavior) {
        const childConnections = connections.filter(c => c.from === behavior.id)
        const childBehaviors = childConnections
            .map(c => behaviors.find(b => b.id === c.to))
            .filter(b => !!b)
            .sort((a, b) => a.position.x < b.position.x)
            .map(recursively)

        return {
            id: behavior.id,
            schema: behavior.schema,
            options: behavior.options,
            children: childBehaviors
        }
    }

    return {
        root: recursively(entryBehavior)
    }
}

export function areEqual(a ,b) {
    return JSON.stringify(a) == JSON.stringify(b)
}
