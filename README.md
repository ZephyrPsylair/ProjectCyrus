  # 🎯 Dynamic Box Spawner - Unreal Engine Internship Task

This project dynamically spawns boxes in the game world using data fetched from a **JSON file hosted online**. Each box has unique properties like color, health, and score, and reacts to player interaction (e.g., shooting them reduces health and increases score).

---

## 🚀 Features

- 📡 **Fetch JSON Data**: Uses Unreal's native HTTP API to fetch box data at runtime.
- 🎨 **Dynamic Box Spawning**: Each box spawns with its type-based attributes:
  - Custom color (using dynamic materials)
  - Health value
  - Score reward on destruction
- 🎯 **Damage System**: Player projectiles reduce box health.
- 🧠 **Score System**: Score is tracked and updated in a custom UI widget when boxes are destroyed.

---

## 🧠 Logic Breakdown

### 1. `ATestGameMode::FetchJson()`
- Makes an HTTP GET request to the provided GitHub-hosted JSON file.
- Parses the `types` array for box definitions (color, health, score).
- Parses the `objects` array for spawn transforms and types.
- Spawns each box actor at its given transform with its respective type data.

### 2. `ABoxActor`
- Holds properties for health, score, and color.
- Uses `UMaterialInstanceDynamic` to dynamically set box color from JSON.
- Implements `TakeDamage()` to apply incoming damage, update score, and destroy when health is depleted.

### 3. `UScoreWidget`
- A UMG Blueprint widget with a single event `BP_UpdateScore` for reflecting score changes.
- Called whenever a box is destroyed.

### 4. `ShooterProjectile.cpp`
- Applies damage to `ABoxActor` on collision using `UGameplayStatics::ApplyDamage`.

---

## 💥 How to Test

1. **Run the Project** in the editor.
2. At `BeginPlay`, JSON data is fetched.
3. Boxes spawn at specified locations with unique colors.
4. Shoot boxes by pressing left mouse button.
5. Watch the **Score UI** update as boxes are destroyed.
6. Logs will show any parsing, spawning, or HTTP errors (if any).

---

## 🤯 Challenges Faced

- Understanding Unreal's native **HTTP module** (no plugins used).
- Learning how to **bind dynamic data** (JSON) to in-game actors.
- Implementing **runtime material updates** using `UMaterialInstanceDynamic`.
- Ensuring **modularity** between data parsing, spawning, and damage logic.

---

## 🛠 Tech Stack

- **Engine**: Unreal Engine 5.6 (C++)
- **JSON Source**: [GitHub Raw Link](https://raw.githubusercontent.com/CyrusCHAU/Varadise-Technical-Test/refs/heads/main/data.json)
- **No plugins used** – fully powered by Unreal’s built-in modules
        
---

## ✅ Done & Dusted

- [x] JSON Fetching
- [x] Parsing & Actor Spawning
- [x] Dynamic Colors
- [x] Health/Damage System
- [x] Score UI

---


