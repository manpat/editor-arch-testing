
Model
=====

Scene
-----
- owns all entities in the scene

Terrain
-------
- responds to transform changes of things that affect terrain

Selection Model
---------------
- stores selected entities
- NOTE: *could* be handled w/ a Selection component possibly. but then do I want view related components in the Scene?
	- if the addition of this component is facilitated by something other than the view, this could work actually


Views
=====

Scene View
----------
- some visual representation of entities + selection
- should be able to make changes through this view
- should update w/ selection, and should be able to modify selection

Entity List View
----------------
- should update w/ selection, and should be able to modify selection

Terrain View
------------
- can just be a texture
- should maybe show update history?
- needs to update in realtime

Component View
--------------
- lists components of selected entity
- should be able to make changes through this view
- should update w/ selection
