this is engine done by David Postnikov as university project

whole engine rendering based on objects ierarchy. classes in this ierarchy build up from some base classes which i call modifiers, for example 2d image square class would be made out of: Instance(basically main thing for ierarchy system, a class that works with children and parents of object), Instance2DData(class that hold minimal data necesary for rendering 2d object), Image2D(class for image)

most modifiers cant work without eachother, for example modifier Instance is independable but Instance2DData is depends on Instace and Image2D depens on Instance2DData, and important part is that they dont have any inheritance

rendering works a little complicated, every time object that needs to be renderer changes parent or its rendering priority, it will be moved in filtered order for rendering, all rendering objects like window or some frame have filtered order in them, then when rendering function is called basically it just iterates through this filtered order and draw each object
