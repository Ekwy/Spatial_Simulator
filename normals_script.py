import bpy
import bmesh
from mathutils import Vector

def draw_func(self, context):
    layout = self.layout

    me = context.object.data

    if me.is_editmode:
        bm = bmesh.from_edit_mesh(me)
        if bm.faces.active is not None:
            row = layout.row()
            col = row.column()
            col.prop(context.window_manager, "active_face_normal", text="", expand=True)
            row.prop(context.window_manager, "active_face_normal", text="")

            if context.window_manager.active_face_normal != bm.faces.active.normal:
                context.window_manager.active_face_normal = bm.faces.active.normal

    else:
        p = me.polygons
        if p.active is not None:
            row = layout.row()
            col = row.column()
            col.prop(p[p.active], "normal", text="", expand=True)
            row.prop(p[p.active], "normal", text="")


def upd_normal(self, context):
    bm = bmesh.from_edit_mesh(context.object.data)
    bm.faces.active.normal = self.active_face_normal.normalized()

def register():
    bpy.types.DATA_PT_normals.prepend(draw_func)
    bpy.types.WindowManager.active_face_normal = bpy.props.FloatVectorProperty(subtype='DIRECTION', precision=3, update=upd_normal)


def unregister():
    bpy.types.DATA_PT_normals.remove(draw_func)
    del bpy.types.WindowManager.active_face_normal



if __name__ == "__main__":
    register()