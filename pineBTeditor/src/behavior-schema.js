export async function loadBehaviorSchemas() {
    return [
        {
            "name": "Selector",
            "hierarchy": "many",
        },
        {
            "name": "Parallel",
            "hierarchy": "many",
        },
        {
            "name": "Sequence",
            "hierarchy": "many",
        },
        {
            "name": "MockCondition",
            "hierarchy": "one",
        },
        {
            "name": "MockTask",
            "hierarchy": "none",
        },
    ]
}
