# Dungeon Master

A game made in raylib.

I started this project initially with the intention of expirmenting with functional reactive animation, or Fran, but ended up making a game. This project might be espically useful for anyone also interested in functional reactive animation for games and would like to see an example of its implementation while also demonstrating some capabilities.

## What is Fran?
Fran is not Functional Reactive Programmng. FRP is a programming paradigm loosely inspired by Fran.
Fran is a collection of types and functions for composing richly interactive animations. All of these types and functions come to gether based on some shared assumptions which allow for the ability to "model" an animation.

We can model the path a bone tavels in an animation as a function: F(t) -> Vector3, where t is a valid number between the range of 0 and the length of the animation. How t = Vector3 isn't relevant, however the ability to model that transformation as a function over time is extremely powerful.
We call these kinds of functions **Behaviour**, and we treat these like variables. As the intended use of a behaviour is to call its function on each tick of the animation, moving the animation forward with the behaviours making up the animation.

Another very important concept to Fran are events, which are a little of what you expect with a helping of more Fran specific stuff. Events allow for sets of arbitrary complex conditions to exist inside the functional space. Many people might mistake this as state, it is not state. If you think it is state you will end up with funcitonal reactive programming and be very very sad. Events are little out of scope for the project as of now, but they will be coming.



## Road Map
 - [ ] **Finish Implementation of Render System**
    - *to properly expirment with Fran the best method seems to be exposing a DrawTick loop that Fran types and functions are used within*
- [ ] **Create Function to Handle Sprite Sheets**
- [ ] **Implement Fran Module**
- [ ] **Implement Fran Events**
    - *while behaviours are implemented in the Fran module, events are not*
    - *everything about Fran is conceptually dense and difficult to implement so this will become a priority as necassary*
- [ ] **Create a Particle System Based on Fran**
    - *this is will be a good limit test and likely force some optimization strategies to emerge*
