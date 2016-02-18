DRAGONFLY - a text-based game engine

Mark Claypool, WPI
Version 4.1, 2016

Additional information available at:

  http://dragonfly.wpi.edu/


LICENSE

Dragonfly is free! This means free of charge for personal
use. Dragonfly may be copied and distributed, provided that this
copyright notice is kept intact. There cannot be a charge for the use
of Dragonfly to anyone else. The copyright holders (Mark Claypool and
WPI) reserve the right to reclassify this software as non-freeware at
a later date (e.g., as shareware). Doing so will not modify the
license agreement of any previously distributed executables.


DEVELOPMENT

Dragonfly has been developed and tested on Linux, Microsoft Windows in
Visual Studio and Mac OS in Xcode.  Brief instructions for setting up
a development environment are provided online:

http://dragonfly.wpi.edu/engine/index.html#setup


NOTES ON DEVELOPING WITH DRAGONFLY

For development help in the form of questions and answers, check
out the Dragonfly Q&A forum at http://alpheus.wpi.edu/dqa/.

If making games beyond the Tutorial, some specific notes about
Dragonfly use:

+ For help with debugging, try RMS' gdb tutorial
(http://www.unknownroad.com/rtfm/gdbtut/gdbtoc.htm).  Or, you might
try DDD (http://www.gnu.org/software/ddd/) as a front end to gdb.

+ Generally, most Dragonfly functions return 0 if they are successful,
and return a non-zero number (usually -1) if there is a problem.  Most
problems are accompanied by a corresponding note in the logfile
(dragonfly.log).

+ Game (and view) objects are handled as pointers by Dragonfly.  Thus,
upon instantiation, objects allocated (via new) by the programmers
game code and then pointers to the objects are passed into the
WorldManager.  Objects *must* be added to the world in this way if
Dragonfly is to act upon them.

+ Game objects should not delete themselves or others, but should
rather add themselves to delete queue (via
WorldManager.markForDelete()).  While ViewObjects can safely be
deleted in a normal fashion, they can also be deleted by
markForDelete().

+ When an Object crosses from inside the game world (specified by
(x,y) not by the bounding box) to outside the game world (outside of
the WorldManager boundary) an "outofbounds" event is sent to the
object.  Note, this only happens once when and Object moves outside
and does repeat itself if it stays outside, nor is there any
corresponding event generated when an outside object comes in.
Objects do not need to register for "outofbounds" events, but get
these automatically (they can, of course, be ignored).

+ Objects do not need to register for collision events, but get
these automatically if they are solid Objects (they can, of course, be
ignored).  ViewObjects do not get collision events.

+ When there is a collision between two Objects, a collision event
object (EventCollision) is send to *both* Objects.  The Object that
was moving (and caused the collision) is always returned via the
Object1() method and the object it hits via the Object2() method.

+ User-defined events that are of interest via the
Object::registerInterest() are added to the WorldManager.  This means
the WorldManager should be used for any onEvent() calls to them.

+ Objects do not need to have sprites to display characters on the
screen.  They can override the draw() event and then manually draw (or
not) any characters they want on the screen using the
GraphicsManager::drawCh() method.

+ The scene graph is organized to make drawing and collision detection
in the game loop efficient.  The scene graph clusters Objects by their
visibility, solidness and altitude.  It assumes that changes to these
properties (e.g., changing altitude) are infrequent and is not designed
to be particularly efficient in doing so.  

