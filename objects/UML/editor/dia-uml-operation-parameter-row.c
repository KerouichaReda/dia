#include "dia-uml-operation-parameter-row.h"
#include "dia_dirs.h"

G_DEFINE_TYPE (DiaUmlOperationParameterRow, dia_uml_operation_parameter_row, GTK_TYPE_LIST_BOX_ROW)

enum {
  UML_OP_PROW_PROP_PARAMETER = 1,
  UML_OP_PROW_N_PROPS
};
static GParamSpec* uml_op_prow_properties[UML_OP_PROW_N_PROPS];

static void
dia_uml_operation_parameter_row_finalize (GObject *object)
{
  DiaUmlOperationParameterRow *self = DIA_UML_OPERATION_PARAMETER_ROW (object);

  g_object_unref (self->parameter);
}

static gboolean
direction_to (GBinding *binding,
              const GValue *from_value,
              GValue *to_value,
              gpointer user_data)
{
  gchar *name = g_value_get_string (from_value);

  if (g_strcmp0 (name, "in") == 0) {
    g_value_set_int (to_value, UML_IN);
  } else if (g_strcmp0 (name, "out") == 0) {
    g_value_set_int (to_value, UML_OUT);
  } else if (g_strcmp0 (name, "inout") == 0) {
    g_value_set_int (to_value, UML_INOUT);
  } else {
    g_value_set_int (to_value, UML_UNDEF_KIND);
  }
  
  return TRUE;
}

static gboolean
direction_from (GBinding *binding,
                const GValue *from_value,
                GValue *to_value,
                gpointer user_data)
{
  int mode = g_value_get_int (from_value);

  if (mode == UML_IN) {
    g_value_set_static_string (to_value, "in");
  } else if (mode == UML_OUT) {
    g_value_set_static_string (to_value, "out");
  } else if (mode == UML_INOUT) {
    g_value_set_static_string (to_value, "inout");
  } else {
    g_value_set_static_string (to_value, "undefined");
  }
  
  return TRUE;
}

static void
dia_uml_operation_parameter_row_set_property (GObject      *object,
                                       guint         property_id,
                                       const GValue *value,
                                       GParamSpec   *pspec)
{
  DiaUmlOperationParameterRow *self = DIA_UML_OPERATION_PARAMETER_ROW (object);
  switch (property_id) {
    case UML_OP_PROW_PROP_PARAMETER:
      self->parameter = g_value_dup_object (value);
      g_object_bind_property (G_OBJECT (self->parameter), "name",
                              G_OBJECT (self->name), "text",
                              G_BINDING_BIDIRECTIONAL | G_BINDING_SYNC_CREATE);
      g_object_bind_property (G_OBJECT (self->parameter), "type",
                              G_OBJECT (self->type), "text",
                              G_BINDING_BIDIRECTIONAL | G_BINDING_SYNC_CREATE);
      g_object_bind_property (G_OBJECT (self->parameter), "value",
                              G_OBJECT (self->value), "text",
                              G_BINDING_BIDIRECTIONAL | G_BINDING_SYNC_CREATE);

      g_object_bind_property_full (G_OBJECT (self->parameter), "kind",
                                   G_OBJECT (self->direction), "active-id",
                                   G_BINDING_BIDIRECTIONAL | G_BINDING_SYNC_CREATE,
                                   direction_from,
                                   direction_to,
                                   NULL, NULL);

      g_object_bind_property (G_OBJECT (self->parameter), "comment",
                              G_OBJECT (self->comment), "text",
                              G_BINDING_BIDIRECTIONAL | G_BINDING_SYNC_CREATE);

      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

static void
dia_uml_operation_parameter_row_get_property (GObject    *object,
                                              guint       property_id,
                                              GValue     *value,
                                              GParamSpec *pspec)
{
  DiaUmlOperationParameterRow *self = DIA_UML_OPERATION_PARAMETER_ROW (object);
  switch (property_id) {
    case UML_OP_PROW_PROP_PARAMETER:
      g_value_set_object (value, self->parameter);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

static void
move_up (DiaUmlOperationParameterRow *self)
{
  GtkWidget *list;
  int index;

  /*
   * If we ever find ourselves in something other than GtkListBox we are
   * in trouble but as a GtkListBoxRow that shouldn't happen, storing the
   * new state is left to the GtkListBox or it's owner
   */
  list = gtk_widget_get_parent (GTK_WIDGET (self));
  index = gtk_list_box_row_get_index (GTK_LIST_BOX_ROW (self));

  g_object_ref (self);
  gtk_container_remove (GTK_CONTAINER (list), GTK_WIDGET (self));
  gtk_list_box_insert (GTK_LIST_BOX (list), GTK_WIDGET (self), index - 1);
  g_object_unref (self);
}

static void
move_down (DiaUmlOperationParameterRow *self)
{
  GtkWidget *list;
  int index;
  
  list = gtk_widget_get_parent (GTK_WIDGET (self));
  index = gtk_list_box_row_get_index (GTK_LIST_BOX_ROW (self));

  g_object_ref (self);
  gtk_container_remove (GTK_CONTAINER (list), GTK_WIDGET (self));
  gtk_list_box_insert (GTK_LIST_BOX (list), GTK_WIDGET (self), index + 1);
  g_object_unref (self);
}

static void
dia_uml_operation_parameter_row_class_init (DiaUmlOperationParameterRowClass *klass)
{
  GFile *template_file;
  GBytes *template;
  GError *err = NULL;
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->finalize = dia_uml_operation_parameter_row_finalize;
  object_class->set_property = dia_uml_operation_parameter_row_set_property;
  object_class->get_property = dia_uml_operation_parameter_row_get_property;

  uml_op_prow_properties[UML_OP_PROW_PROP_PARAMETER] =
    g_param_spec_object ("parameter",
                         "Parameter",
                         "Parameter this editor controls",
                         DIA_UML_TYPE_PARAMETER,
                         G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);

  g_object_class_install_properties (object_class,
                                     UML_OP_PROW_N_PROPS,
                                     uml_op_prow_properties);

  /* TODO: Use GResource */
  template_file = g_file_new_for_path (build_ui_filename ("ui/dia-uml-operation-parameter-row.ui"));
  template = g_file_load_bytes (template_file, NULL, NULL, &err);

  if (err)
    g_critical ("Failed to load template: %s", err->message);

  gtk_widget_class_set_template (widget_class, template);
  gtk_widget_class_bind_template_child (widget_class, DiaUmlOperationParameterRow, name);
  gtk_widget_class_bind_template_child (widget_class, DiaUmlOperationParameterRow, type);
  gtk_widget_class_bind_template_child (widget_class, DiaUmlOperationParameterRow, value);
  gtk_widget_class_bind_template_child (widget_class, DiaUmlOperationParameterRow, direction);
  gtk_widget_class_bind_template_child (widget_class, DiaUmlOperationParameterRow, comment);
  gtk_widget_class_bind_template_callback (widget_class, move_up);
  gtk_widget_class_bind_template_callback (widget_class, move_down);

  g_object_unref (template_file);
}

static void
dia_uml_operation_parameter_row_init (DiaUmlOperationParameterRow *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
}

GtkWidget *
dia_uml_operation_parameter_row_new (DiaUmlParameter *op)
{
  return g_object_new (DIA_UML_TYPE_OPERATION_PARAMETER_ROW,
                       "parameter", op, 
                       NULL);
}