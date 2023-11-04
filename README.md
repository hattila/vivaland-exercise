# Vivaland Exercise DevLog

Created a map, set up NavMesh. Added navigable slopes.

## TankCharacter setup

- Added Camera and Spring Arm from C++.
- Character rotate toward Destination
- Camera don't follow rotation
- Click to move binding.

Navigation is done with `UAIBlueprintHelperLibrary::SimpleMoveToLocation`
The TopDownExample project uses the same, and I've found that it is a simple enough solution for the time being (no need to visualize path etc. with the Navigation component)

Replicated movement - Done by the `CharacterMovementComponent`
To achieve smooth replicated movement, I've enabled client side Navigation. Server will corret this anyway, but the movement of the client won't be jerky this way. (Engine - Navigation System)

DefaultEngine.ini
```
[/Script/NavigationSystem.NavigationSystemV1]
bAllowClientSideNavigation=True
```

I've interpreted "Smooth Camera" as enabling camera lag with a really low value for emphasis.

Added acceleration and deceleration to the Vehicle for an even smoother experience.

## Combat setup

Created a Projectile with a `ProjectileMovementComponent` on it. I left it mostly on it's defaults, so the projectile is affected by gravity, and there is no infinite range.

Controller listens to Shoot Action and calls Server Side RPC Shoot on the controlled TankCharacter. The ProjectileMovementComponent takes care of the replication so shooting is multiplayer ready.

There is no Health and Damage concept because the requirement is just One Shotting each other. The TankCharacter dies `OnTakeAnyDamage`.

### Kills, respawning and scoring

- Projectile OnHit deals damage to Characters
- Character dies on any damage (Destroy) -> notifies it's Controller
- Controller UnPossess, notify GameMode about the death of it's Pawn, with extra of of who killed it
- GameMode shows a debug message about who killed who
- GameMode tells the GameState to add score to the killer.
- GameMode chooses a random start location, spawns new Pawn for the dead player's Controller.
- GameState calculates the new score and __orders the scores DESC__.
- GameState replicates scores to all clients
- GameState calls Controllers to refresh scores
- Client side Controllers own Scoreboard Widgets and re renders the scores


## Additional notes

I use GitHub CoPilot. I left some prompts in to indicate snippets that were created or initialized by CoPilot. Most notable examples are in the `DeathMatchGameState.cpp` and `ScoreboardWidget.cpp`.


### Example CMDs for testing

Run dedicated server locally:
```
C:\UnrealEngineVersions\UE_5.1\Engine\Binaries\Win64\UnrealEditor.exe "H:\UnrealProjects\VivalandExercise\VivalandExercise.uproject" /Game/Map/VivalandExerciseMap?listen -server -log
```
Run game instance joining a local Server:
```
C:\UnrealEngineVersions\UE_5.1\Engine\Binaries\Win64\UnrealEditor.exe "H:\UnrealProjects\VivalandExercise\VivalandExercise.uproject" 127.0.0.1 -game -log
```
Run game instance solo:
```
C:\UnrealEngineVersions\UE_5.1\Engine\Binaries\Win64\UnrealEditor.exe "H:\UnrealProjects\VivalandExercise\VivalandExercise.uproject" -game -log
```


### Used literature (most notable):

Enhanced Input 
https://nightails.com/2022/10/16/unreal-engine-enhanced-input-system-in-c/

More robust nav implementation could be:
https://snorristurluson.github.io/ClickToMove/

Deceleration
https://www.reddit.com/r/unrealengine/comments/x48yyd/how_to_smoothly_stop_ais_movement/

Click to move after respawn
https://forums.unrealengine.com/t/simple-move-not-working-with-possessed-characters/437136/11

