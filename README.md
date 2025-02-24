# Battle Arena Board Game 🎮⚡

## 📖 **Project Overview**

**Battle Arena Board Game** is a hybrid tabletop game designed to bring the depth and strategy of **MOBA (Multiplayer Online Battle Arena) games**—like *Dota 2* and *League of Legends*—to the physical world.  

This game blends **traditional board game mechanics** with **modern technology** to recreate key MOBA elements, such as:  
- ✅ **Fog of War** and limited vision mechanics  
- ✅ **Automated minion waves and lane control**  
- ✅ **Hero abilities, cooldown management, and item effects**  

At its core, this project combines physical interaction with automated gameplay, using hardware to handle tedious tasks while leaving strategy and fun in the hands of the players.  

---

## 🔧 **Key Features**

- 🎛️ **16×16 RGB LED Matrix** to represent the game map and hero movements  
- 🎡 **Motorized rotating and tilting board** to simulate Fog of War mechanics dynamically  
- 💻 **ESP32 microcontroller** to handle game logic, minion automation, and state management  
- 📲 **5-inch TFT touchscreen** for player interactions, status updates, and in-game controls  
- 🎴 **RFID card system** (optional) for triggering abilities and using items  

---

## 💡 **How It Works**

1. **Game Initialization:**  
   The ESP32 initializes the LED matrix, sets up player turns, and controls the touchscreen interface.  

2. **Fog of War Mechanics:**  
   - The board tilts **45°** toward the active team during their turn, hiding the map from opponents.  
   - When two heroes are close enough to "see" each other, the board flattens to reveal the shared area.  

3. **Minion Automation:**  
   - Minions automatically spawn and push lanes based on pre-programmed AI behavior.  
   - The microcontroller manages movements, attacks, and lane pressure autonomously.  

4. **Player Actions:**  
   - Players can interact with the touchscreen to move, attack, or activate abilities.  
   - Optional: RFID-enabled cards allow players to scan abilities and items directly onto the board.  

5. **Turn Rotation:**  
   - At the end of a turn, the board rotates **180°** to face the opposing team.  
