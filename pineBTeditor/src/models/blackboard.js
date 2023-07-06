export function newBlackboard() {
    return {
        entries: [...Array(20).keys()].map(i => ({
            key: i,
            name: `test ${i}`,
            value: i + 100
        }))
    }
}
