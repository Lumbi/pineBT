export async function loadBehaviorSchemas() {
    return [
        {
            "name": "Selector",
            "hierarchy": "many",
            "options" : {
                "live" : "boolean"
            }
        },
        {
            "name": "Parallel",
            "hierarchy": "many",
            "options": {
                "success_policy": 2,
                "failure_policy": 2
            }
        },
        {
            "name": "Sequence",
            "hierarchy": "many",
        },
        {
            "name": "MockCondition",
            "hierarchy": "one",
            "options": {
                "mode": 2,
                "negate": "boolean",
                "force_check": "boolean"
            }
        },
        {
            "name": "MockTask",
            "hierarchy": "none",
            "options": {
                "force_result": 4
            }
        },
    ]
}
