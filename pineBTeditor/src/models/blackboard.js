import { numericallyAscending } from '../sort'

export function create() {
    return {
        entries: [...Array(20).keys()].map(i => ({
            key: i + 1,
            name: `test ${i}`,
            value: i + 100
        }))
    }
}

export function addingNewEntry(blackboard) {
    const nextAvailableKey = blackboard.entries
        .map(_ => _.key)
        .sort(numericallyAscending)
        .reduce((previous, current) => previous == current ? current + 1 : previous, 1)

    const newEntry = {
        key: nextAvailableKey,
        name: '',
        value: 0
    }

    return {
        ...blackboard,
        entries: [...blackboard.entries, newEntry]
    }
}

export function updatingEntry(blackboard, entryUpdate) {
    return {
        ...blackboard,
        entries: blackboard.entries.map(entry =>
            entry.key === entryUpdate.key 
                ? { ...entry, ...entryUpdate }
                : entry
        )
    }
}

export function removingEntry(blackboard, key) {
    return {
        ...blackboard,
        entries: blackboard.entries.filter(_ => _.key !== key)
    }
}
