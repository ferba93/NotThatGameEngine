# NotThatGameEngine
An engine for CITM's Engine project which focuses on skeletical animation

## Creators:
<li>Me (Hodu): [Github link](https://github.com/ferba93)</li>
<li>Lots of [hopes and dreams](https://images-na.ssl-images-amazon.com/images/I/91Hv6KacesL.jpg)</li>

## Instructions:
- Camera controls:<br/>
	<ul>
	<li>Right click + WASD & QE to move camera</li>
	<li>F to center an object</li>
	<li>Alt + Left click to rotate around object</li>
	<li>Mouse wheel to zoom (when scene is focused)</li>
	<li>Holding shift duplicates movement speed</li>
	</ul>
- All menus can be re-opened via Upper Menu tabs<br/>
- Drag a model on the application to load it on screen<br/>
- Drag a texture on the application to load it. If there is an object selected, it will apply that texture to the model<br/>

## Additional functionality:
- Windows have docking options enabled<br/>
- Textures are not loaded more than once<br/>
- Transforms input don't take other values but numbers and operators<br/>
- Transform input alters object transforms (word of warning: input of +90 on Y rotation makes object go party hard)<br/>
- Trees can be moved through with keyboard input (but focus is only on click)<br/>

## Additional comments:
- Obligued path: the directory must be a folder named "Assets/" <br/>
- Obligued path for textures: the directory must be "Assets/Library/Textures/" <br/>
- Do not move Checkers, Alex or Degenerate textures. They are internally loaded for the user to have by default, thus the path is hardcoded :) <br/>
- Primitives do not have normals nor texture coordinates, thus they can't recieve a texture <br/>
- Images can't have the same name, even if they are of different formats<br/>
- Give the dragon time to load :')<br/>





