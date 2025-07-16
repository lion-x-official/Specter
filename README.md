# Specter - CS2 External Cheat (Pre-Alpha)

**Specter** is an external cheat for Counter-Strike 2 (CS2), currently in **pre-alpha** (version 0.0.9). This project serves as a prototype for interacting with CS2’s memory to provide cheat functionalities. This release represents a **major update** with significant structural changes and new features.

⚠️ **Disclaimer**: Specter is a proof-of-concept for educational purposes only. Use responsibly and in compliance with game terms of service. The developers are not liable for any misuse or consequences.

## Features
- **Process Detection**: Attaches to `cs2.exe` (x64 architecture only).  
- **Dynamic Offset System**: Automatically updates game offsets using memory for compatibility with CS2 patches.  
- **Entity List Interaction**: Reads and processes CS2’s EntityList for in-game entity data.  
- **Entity Position Display**: Shows real-time positions of all entities (requires being on a map).  
- **Trigger Bot Foundation**: Initial framework for a trigger bot (in development).  
- **Player Health Display**: Displays the player’s health in-game (requires being on a map).  
- **Enhanced Logging**: Colored console logs for clear debugging and operation tracking.  
- **ProcessHelper**: Robust system for efficient memory operations.  
- **Console Title**: Custom console title for easy identification.  
- **Global Variables & Math Utilities**: Added for streamlined development and calculations.  
- **Proprietary License**: Governs the use and distribution of Specter.

## Requirements
- **Operating System**: Windows (x64 architecture only, matching CS2’s requirements).  
- **Game**: Counter-Strike 2 (`cs2.exe`) running and on a map for entity and health features.  
- **Compiler**: C++ compiler supporting C++17 or later (e.g., MSVC, MinGW).  
- **Dependencies**: None; Specter is standalone.

## Installation
> **Note**: Specter is not yet released for public use. The following steps are for developers or testers.

1. **Clone the Repository**:  
   ```bash
   git clone https://github.com/lion-x-official/Specter
   ```
2. **Build the Project**:  
   - Open in an IDE (e.g., Visual Studio).  
   - Ensure C++17 or later is enabled.  
   - Build for x64 architecture (Release or Debug mode).  
3. **Run Specter**:  
   - Launch CS2 and join a map.  
   - Run the compiled Specter executable.  
   - Check the console for colored logs, player health, and entity positions.

## Current Status
- **Version**: 0.0.9 (Pre-Alpha)  
- **Stage**: Prototype with experimental features.  
- **Known Limitations**:  
  - Requires x64 system.  
  - Entity and health features work only on a CS2 map.  
  - Limited cheat functionality; more features planned.  
- **Note**: The old repository was removed, and this is a fresh start. We apologize for any inconvenience.

## Contributing
Contributions are welcome! To contribute:  
1. Fork the repository.  
2. Create a branch (`git checkout -b feature/your-feature`).  
3. Commit changes (`git commit -m "Add your feature"`).  
4. Push to your branch (`git push origin feature/your-feature`).  
5. Open a pull request with a detailed description.  

Ensure code follows the project’s structure and includes detailed comments.

## License
Specter is distributed under a **proprietary license**. See the `LICENSE` file for details.

## Roadmap
- Complete trigger bot implementation.  
- Add advanced cheat features (e.g., aimbot, wallhack).  
- Enhance stability and error handling.  
- Optimize memory operations and offset management.

## Disclaimer
Specter is a **pre-alpha prototype** and may contain bugs or incomplete features. Use at your own risk. The developers are not responsible for any consequences, including bans or legal issues.

Thank you for checking out Specter! Feedback is appreciated.  

🔗 [GitHub Repository](https://github.com/lion-x-official/Specter)