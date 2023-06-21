# pineBT

A C++ implementation of behavior trees

```cpp
auto behaviorTree = BehaviorTree::build()
	.select()
		.conditon(PlayerVisible)
		.conditon(CanFire)
			.task(FireAtPlayer)
		.sequence()
			.task(MoveToPlayer)
			.task(Wait)
		.close()
		.task(Idle)
	.close()
.end();
```
