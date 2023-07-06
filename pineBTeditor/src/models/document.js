import { rootBehavior, isRoot } from './behavior'
import * as Blackboard from './blackboard'

export function loadSchemas(document) {
    const schemas = Object.values(pineBT.schemas())
    document.setSchemas(schemas)
}

export function reset(document) {
    document.setFilePath(undefined)
    document.setSavedData(undefined)
    document.setBehaviors([rootBehavior()])
    document.setConnections([])
}

export function open(document, file) {
    const { path, data } = file
    const { behaviors, connections } = JSON.parse(data)
    document.setBehaviors(behaviors)
    document.setConnections(connections)
    document.setFilePath(path)
    document.setSavedData(undefined)
}

export function findBehaviorWithId(document, id) {
    const { behaviors } = document
    return behaviors.find(b => b.id === id)
}

export function schemaForBehavior(document, behavior) {
    const { schemas } = document
    if (!behavior) { return undefined }
    return schemas.find(s => s.name === behavior.schema)
}

export function newBehaviorFromSchema(document, schema) {
    const { behaviors } = document

    const maxBehaviorId = behaviors.map(b => b.id).sort().slice(-1)[0]
    const nextBehaviorId = maxBehaviorId + 1

    function defaultValueForOptionType(type) {
        if (type === 'boolean') {
            return false
        } else if (type === 'number') {
            return 0
        } else if (typeof type === 'number') { // enumeration
            return { case: 0 }
        }
        return undefined
    }

    const newBehavior = {
        schema: schema.name,
        id: nextBehaviorId,
        position: { x: 0, y: 0 },
        options: schema.options && Object.fromEntries(
            Object.entries(schema.options).map(([key, type]) => [
                key,
                defaultValueForOptionType(type)
            ])
        )
    }

    return newBehavior
}

export function addBehavior(document, behavior) {
    const { setBehaviors, behaviors } = document
    setBehaviors([...behaviors, behavior])
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

export function updateBehaviorStatuses(document, statuses) {
    const { behaviors, setBehaviors } = document
    setBehaviors(
        behaviors.map(b => {
            const found = statuses.find(s => s.id === b.id)
            return { ...b, status: found && found.status && found.status.toLowerCase() }
        })
    )
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

export function allowsChildren(document, behavior) {
    const schema = schemaForBehavior(document, behavior)
    if (isRoot(behavior)) {
        return true
    } if (!schema || !schema.hierarchy) {
        return false
    } else if (schema.hierarchy === 'none') {
        return false
    }
    return true
}

export function childrenOfBehavior(document, behavior) {
    const { behaviors, connections } = document
    if (!behavior) { return [] }

    const childConnections = connections
        .filter(c => c.from === behavior.id)

    const childBehaviors = childConnections
        .map(c => c.to)
        .flatMap(to => behaviors.filter(b => b.id === to))

    return childBehaviors
}

export function canAddChildToBehavior(document, behavior) {
    const { schemas } = document

    if (!behavior) { return false }
    if (isRoot(behavior)) { return childrenOfBehavior(document, behavior).length === 0 }

    const schema = schemas.find(s => s.name === behavior.schema)
    if (!schema) { return false }

    if (schema.hierarchy === 'none') {
        return false
    } else if (schema.hierarchy === 'one') {
        return childrenOfBehavior(document, behavior).length === 0
    } else if (schema.hierarchy === 'many') {
        return true
    }

    return false
}

export function canAddConnection(document, connection) {
    const { connections } = document
    const alreadyExists = !!connections.find(c => c.from === connection.from && c.to === connection.to)
    const connectionToSelf = connection.from === connection.to
    const alreadyHasParent = !!connections.find(c => c.to === connection.to)
    const parentBehavior = findBehaviorWithId(document, connection.from)
    const canAddChild = canAddChildToBehavior(document, parentBehavior)
    const isValid = !alreadyExists && 
                    !connectionToSelf && 
                    !alreadyHasParent &&
                    canAddChild
    return isValid
}

export function addConnection(document, connection) {
    const { connections, setConnections } = document
    if (canAddConnection(document, connection)) {
        setConnections([...connections, connection])
    }
}

export function deleteConnection(document, connection) {
    const { setConnections, connections } = document
    const { from, to } = connection
    setConnections(
        connections.filter(c => c.from !== from || c.to !== to)
    )
}

export function addBlackboardEntry(document) {
    const { blackboard, setBlackboard } = document
    setBlackboard(Blackboard.addingNewEntry(blackboard))
}

export function deleteBlackboardEntry(document, key) {
    const { blackboard, setBlackboard } = document
    setBlackboard(Blackboard.removingEntry(blackboard, key))
}

export function toData(document) {
    const { behaviors, connections } = document
    const persistentBehaviors = behaviors.map(b => ({
        ...b,
        status: undefined
    }))
    return JSON.stringify(
        {
            behaviors: persistentBehaviors,
            connections,
        },
        undefined,
        4
    )
}

export async function save(document, newPath) {
    const documentData = toData(document)
    const path = newPath || document.filePath
    const result = await window.electron.saveFile({
        path: path,
        data: documentData
    })
    if (result.path) {
        document.setFilePath(result.path)
        document.setSavedData(documentData)
    }
}
