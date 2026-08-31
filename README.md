# Dungeon Master

A 2D tactical game built with raylib, blending **bullet-chess** with **rpg elements**.
Command units on a tile-based grid where every piece you unlock changes your available strategies.

> **Note:** This project serves as an experiment to implement **Fran** (Functional Reactive Animation) in a real-time game loop.

## What is Fran?
Fran is not Functional Reactive Programming. FRP is a programming paradigm loosely inspired by Fran.
Fran is a collection of types and functions for composing richly interactive animations. All of these types and functions come together based on some shared assumptions which allow for the ability to "model" an animation.

We can model the path a bone travels in an animation as a function: F(t) -> Vector3, where t is a valid number between the range of 0 and the length of the animation. How t maps to a Vector3 value isn't relevant, what matters is the ability to model the transformation as a function. Since it is the ability to model the transformation as a function which could allow you to theoretically compile animation data into a series of Fran functions.
We call these kinds of functions **Behaviour**, and we treat these like variables. As the intended use of a behaviour is to call its function on each tick of the animation, moving the animation forward with the behaviours making up the animation.

Another very important concept to Fran are events, which are a little of what you expect with a helping of more Fran specific stuff. Events allow for sets of arbitrary complex conditions to exist inside the functional space. Many people might mistake this as state, it is not state. If you think it is state you will end up with functional reactive programming and be very very sad. Events are a little out of scope for the project as of now, but they will be coming.



## Current Roadmap
- [ ] **Implement Fran Module**
- [ ] **Finish Implementation of Render System**
    - *to properly experiment with Fran the best method seems to be exposing a DrawTick loop that Fran types and functions are used within*
- [ ] **Refactor Render System to use Fran Behaviours**
- [x] **Create Function to Handle Sprite Sheets**
- [ ] **Fix RenderData Mutex Idea**
    - *Replace per element mutex with a centralized lock free task graph scheduler*
- [ ] **Fix Singleton Systems Idea**
    - *this is drawing upon the Unreal Engine Subsystems*
    - *needs to be recreated with multi threading in mind, fine for now*
- [ ] **Change Build System to CMake**
- [ ] **Setup Async Render Pipeline** 
- [ ] **Fix Coordinate Mapping**
    - *Flatten grid to 1D vector and respectively update GetTrueCoordinates*
- [ ] **Implement Fran Events**
    - *while behaviours are implemented in the Fran module, events are not*
    - *everything about Fran is conceptually dense and difficult to implement so this will become a priority as necessary*
- [ ] **Create a Particle System Based on Fran**
    - *this will be a good limit test and likely force some optimization strategies to emerge*
    - *this is also likely what Fran will be used for the most on this project to create interesting VFX*

## Building
Currently builds solely from a plain make file with the **make run** command and has only been configured for linux machines with the g++ compiler. As the project develops stronger build support will be implemented. Until then binaries will be released as necessary.
