#include <gtk/gtk.h>
#include "uml.h"

G_BEGIN_DECLS

#define DIA_UML_TYPE_OPERATION_PARAMETER_ROW (dia_uml_operation_parameter_row_get_type ())
G_DECLARE_FINAL_TYPE (DiaUmlOperationParameterRow, dia_uml_operation_parameter_row, DIA_UML, OPERATION_PARAMETER_ROW, GtkListBoxRow)

struct _DiaUmlOperationParameterRow {
  GtkListBoxRow parent;

  GtkWidget *name;
  GtkWidget *type;
  GtkWidget *value;
  GtkWidget *direction;
  GtkTextBuffer *comment;

  DiaUmlParameter *parameter;
};

GtkWidget       *dia_uml_operation_parameter_row_new           (DiaUmlParameter             *parameter);
DiaUmlParameter *dia_uml_operation_parameter_row_get_parameter (DiaUmlOperationParameterRow *self);

G_END_DECLS