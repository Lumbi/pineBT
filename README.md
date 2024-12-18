# pineBT

A C++ Behavior Tree implementation and FFI bridging to an Electron app allowing editing and previewing a simulated BT in real-time

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

And an Electron editor app

<img width="800" alt="Behavior Tree Editor" src="https://github.com/Lumbi/Lumbi/assets/1648852/c9946b7c-4bbb-43b7-a400-ab6d2fbb8df6">

<img width="800" alt="Behavior Tree Editor" src="https://github.com/Lumbi/Lumbi/assets/1648852/4e8affbb-10c0-473a-8329-2e5121039cda">
