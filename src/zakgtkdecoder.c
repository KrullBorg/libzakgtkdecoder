/*
 * ZakGtkDecoder widget for GTK+
 *
 * Copyright (C) 2015 Andrea Zagli <azagli@libero.it>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif

#include <glib/gi18n-lib.h>

#ifdef G_OS_WIN32
	#include <windows.h>
#endif

#include "zakgtkdecoder.h"

enum
{
	PROP_0,
	PROP_SHOW_CLEAN,
	PROP_SHOW_NEW,
	PROP_SHOW_OPEN,
	PROP_SHOW_BROWSE
};

static void zak_gtk_decoder_class_init (ZakGtkDecoderClass *klass);
static void zak_gtk_decoder_init (ZakGtkDecoder *decoder);

static void zak_gtk_decoder_decode (ZakGtkDecoder *decoder);

static void zak_gtk_decoder_size_request (GtkWidget *widget,
                                    GtkRequisition *requisition);
static void zak_gtk_decoder_size_allocate (GtkWidget *widget,
                                     GtkAllocation *allocation);

static void zak_gtk_decoder_set_property (GObject *object,
                                           guint property_id,
                                           const GValue *value,
                                           GParamSpec *pspec);
static void zak_gtk_decoder_get_property (GObject *object,
                                           guint property_id,
                                           GValue *value,
                                           GParamSpec *pspec);

static void zak_gtk_decoder_on_btn_clean_clicked (GtkEntry *entry,
                                         GtkEntryIconPosition icon_pos,
                                         GdkEvent *event,
                                         gpointer user_data);

static void zak_gtk_decoder_on_btn_new_clicked (GtkButton *button,
                                        gpointer user_data);

static void zak_gtk_decoder_on_btn_open_clicked (GtkButton *button,
                                        gpointer user_data);

static void zak_gtk_decoder_on_btn_browse_clicked (GtkButton *button,
                                        gpointer user_data);

static GtkWidgetClass *parent_class = NULL;


#define ZAK_GTK_DECODER_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), GTK_TYPE_FORM_DECODER, ZakGtkDecoderPrivate))

typedef struct _ZakGtkDecoderPrivate ZakGtkDecoderPrivate;
struct _ZakGtkDecoderPrivate
	{
		GtkWidget *hbox;
		GtkWidget *txt_decoded;
		GtkWidget *btn_new;
		GtkWidget *btn_open;
		GtkWidget *btn_browse;
		GtkWidget *lbl_key;
	};

G_DEFINE_TYPE (ZakGtkDecoder, zak_gtk_decoder, GTK_TYPE_BIN);

#ifdef G_OS_WIN32
static HMODULE backend_dll = NULL;

BOOL WINAPI DllMain (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved);

BOOL WINAPI
DllMain (HINSTANCE hinstDLL,
         DWORD     fdwReason,
         LPVOID    lpReserved)
{
	switch (fdwReason)
		{
			case DLL_PROCESS_ATTACH:
				backend_dll = (HMODULE) hinstDLL;
				break;
			case DLL_THREAD_ATTACH:
			case DLL_THREAD_DETACH:
			case DLL_PROCESS_DETACH:
				break;
		}
	return TRUE;
}
#endif

static void
zak_gtk_decoder_class_init (ZakGtkDecoderClass *klass)
{
	GtkWidgetClass *widget_class;

	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	g_type_class_add_private (object_class, sizeof (ZakGtkDecoderPrivate));

	widget_class = (GtkWidgetClass*) klass;
	parent_class = g_type_class_peek_parent (klass);

	object_class->set_property = zak_gtk_decoder_set_property;
	object_class->get_property = zak_gtk_decoder_get_property;

	/*widget_class->size_request = zak_gtk_decoder_size_request;*/
	widget_class->size_allocate = zak_gtk_decoder_size_allocate;

	g_object_class_install_property (object_class, PROP_SHOW_CLEAN,
	                                 g_param_spec_boolean ("show-btn-clean",
	                                                       "Show clean icon",
	                                                       "Whether to show the clean icon",
	                                                       FALSE,
	                                                       G_PARAM_READWRITE));

	g_object_class_install_property (object_class, PROP_SHOW_NEW,
	                                 g_param_spec_boolean ("show-btn-new",
	                                                       "Show new button",
	                                                       "Whether to show the new button",
	                                                       FALSE,
	                                                       G_PARAM_READWRITE));

	g_object_class_install_property (object_class, PROP_SHOW_OPEN,
	                                 g_param_spec_boolean ("show-btn-open",
	                                                       "Show open button",
	                                                       "Whether to show the open button",
	                                                       FALSE,
	                                                       G_PARAM_READWRITE));

	g_object_class_install_property (object_class, PROP_SHOW_BROWSE,
	                                 g_param_spec_boolean ("show-btn-browse",
	                                                       "Show browse button",
	                                                       "Whether to show the browse button",
	                                                       TRUE,
	                                                       G_PARAM_READWRITE));

	/**
	 * ZakGtkDecoder::btn_clean_clicked:
	 * @zak_gtk_decoder:
	 *
	 */
	klass->btn_clean_clicked_signal_id = g_signal_new ("btn-clean-clicked",
	                                               G_TYPE_FROM_CLASS (object_class),
	                                               G_SIGNAL_RUN_LAST,
	                                               0,
	                                               NULL,
	                                               NULL,
	                                               g_cclosure_marshal_BOOLEAN__FLAGS,
	                                               G_TYPE_BOOLEAN,
	                                               1, G_TYPE_INT);

	/**
	 * ZakGtkDecoder::btn_new_clicked:
	 * @zak_gtk_decoder:
	 *
	 */
	klass->btn_new_clicked_signal_id = g_signal_new ("btn-new-clicked",
	                                               G_TYPE_FROM_CLASS (object_class),
	                                               G_SIGNAL_RUN_LAST,
	                                               0,
	                                               NULL,
	                                               NULL,
	                                               g_cclosure_marshal_VOID__VOID,
	                                               G_TYPE_NONE,
	                                               0);

	/**
	 * ZakGtkDecoder::btn_open_clicked:
	 * @zak_gtk_decoder:
	 *
	 */
	klass->btn_open_clicked_signal_id = g_signal_new ("btn-open-clicked",
	                                               G_TYPE_FROM_CLASS (object_class),
	                                               G_SIGNAL_RUN_LAST,
	                                               0,
	                                               NULL,
	                                               NULL,
	                                               g_cclosure_marshal_VOID__VOID,
	                                               G_TYPE_NONE,
	                                               0);

	/**
	 * ZakGtkDecoder::btn_browse_clicked:
	 * @zak_gtk_decoder:
	 *
	 */
	klass->btn_browse_clicked_signal_id = g_signal_new ("btn-browse-clicked",
	                                               G_TYPE_FROM_CLASS (object_class),
	                                               G_SIGNAL_RUN_LAST,
	                                               0,
	                                               NULL,
	                                               NULL,
	                                               g_cclosure_marshal_VOID__VOID,
	                                               G_TYPE_NONE,
	                                               0);
}

static void
zak_gtk_decoder_init (ZakGtkDecoder *decoder)
{
	ZakGtkDecoderPrivate *priv = ZAK_GTK_DECODER_GET_PRIVATE (decoder);

	GtkWidget *icon;

	priv->hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 1);
	gtk_container_add (GTK_CONTAINER (decoder), priv->hbox);
	gtk_widget_show (priv->hbox);

	priv->txt_decoded = gtk_entry_new ();
	gtk_box_pack_start (GTK_BOX (priv->hbox), priv->txt_decoded, TRUE, TRUE, 0);
	gtk_editable_set_editable (GTK_EDITABLE (priv->txt_decoded), FALSE);
	gtk_widget_show (priv->txt_decoded);

	g_signal_connect (G_OBJECT (priv->txt_decoded), "icon-press",
	                  G_CALLBACK (zak_gtk_decoder_on_btn_clean_clicked), (gpointer)decoder);

	priv->btn_new = gtk_button_new ();
	gtk_widget_set_tooltip_text (priv->btn_new, _("New"));
	gtk_box_pack_start (GTK_BOX (priv->hbox), priv->btn_new, FALSE, FALSE, 0);
	gtk_widget_set_no_show_all (priv->btn_new, TRUE);
	icon = gtk_image_new_from_stock ("gtk-new", GTK_ICON_SIZE_MENU);
	gtk_container_add (GTK_CONTAINER (priv->btn_new), icon);
	gtk_widget_show (icon);

	g_signal_connect (G_OBJECT (priv->btn_new), "clicked",
	                  G_CALLBACK (zak_gtk_decoder_on_btn_new_clicked), (gpointer)decoder);

	priv->btn_open = gtk_button_new ();
	gtk_widget_set_tooltip_text (priv->btn_open, _("Open"));
	gtk_box_pack_start (GTK_BOX (priv->hbox), priv->btn_open, FALSE, FALSE, 0);
	gtk_widget_set_no_show_all (priv->btn_open, TRUE);
	icon = gtk_image_new_from_stock ("gtk-open", GTK_ICON_SIZE_MENU);
	gtk_container_add (GTK_CONTAINER (priv->btn_open), icon);
	gtk_widget_show (icon);

	g_signal_connect (G_OBJECT (priv->btn_open), "clicked",
	                  G_CALLBACK (zak_gtk_decoder_on_btn_open_clicked), (gpointer)decoder);

	priv->btn_browse = gtk_button_new ();
	gtk_widget_set_tooltip_text (priv->btn_browse, _("Find"));
	gtk_box_pack_start (GTK_BOX (priv->hbox), priv->btn_browse, FALSE, FALSE, 0);
	gtk_widget_set_no_show_all (priv->btn_browse, TRUE);
	icon = gtk_image_new_from_stock ("gtk-find", GTK_ICON_SIZE_MENU);
	gtk_container_add (GTK_CONTAINER (priv->btn_browse), icon);
	gtk_widget_show (icon);
	gtk_widget_show (priv->btn_browse);

	g_signal_connect (G_OBJECT (priv->btn_browse), "clicked",
	                  G_CALLBACK (zak_gtk_decoder_on_btn_browse_clicked), (gpointer)decoder);

	priv->lbl_key = gtk_label_new ("");
	gtk_box_pack_start (GTK_BOX (priv->hbox), priv->lbl_key, FALSE, FALSE, 0);
	gtk_widget_set_no_show_all (priv->lbl_key, TRUE);
}

/**
 * zak_gtk_decoder_new:
 *
 * Creates a new #ZakGtkDecoder.
 *
 * Returns: The newly created #ZakGtkDecoder widget.
 */
GtkWidget
*zak_gtk_decoder_new ()
{
	GtkWidget *w = GTK_WIDGET (g_object_new (zak_gtk_decoder_get_type (), NULL));

	gchar *localedir;

#ifdef G_OS_WIN32

	gchar *moddir;
	gchar *p;

	moddir = g_win32_get_package_installation_directory_of_module (backend_dll);

	p = g_strrstr (moddir, g_strdup_printf ("%c", G_DIR_SEPARATOR));
	if (p != NULL
	    && (g_ascii_strcasecmp (p + 1, "src") == 0
	        || g_ascii_strcasecmp (p + 1, ".libs") == 0))
		{
			localedir = g_strdup (LOCALEDIR);
		}
	else
		{
			localedir = g_build_filename (moddir, "share", "locale", NULL);
		}

	g_free (moddir);

#else

	localedir = g_strdup (LOCALEDIR);

#endif

	bindtextdomain (GETTEXT_PACKAGE, localedir);
	textdomain (GETTEXT_PACKAGE);
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");

	g_free (localedir);

	return w;
}

void
zak_gtk_decoder_set_key (ZakGtkDecoder *decoder, const gchar *key)
{
	ZakGtkDecoderPrivate *priv;

	g_return_if_fail (GTK_IS_FORM_DECODER (decoder));

	priv = ZAK_GTK_DECODER_GET_PRIVATE (decoder);

	gtk_label_set_text (GTK_LABEL (priv->lbl_key), key);
	zak_gtk_decoder_decode (decoder);
}

gchar
*zak_gtk_decoder_get_key (ZakGtkDecoder *decoder)
{
	ZakGtkDecoderPrivate *priv;

	g_return_val_if_fail (GTK_IS_FORM_DECODER (decoder), NULL);

	priv = ZAK_GTK_DECODER_GET_PRIVATE (decoder);

	return g_strdup (gtk_label_get_text (GTK_LABEL (priv->lbl_key)));
}

gchar
*zak_gtk_decoder_get_decoded (ZakGtkDecoder *decoder)
{
	ZakGtkDecoderPrivate *priv;

	g_return_val_if_fail (GTK_IS_FORM_DECODER (decoder), NULL);

	priv = ZAK_GTK_DECODER_GET_PRIVATE (decoder);

	return g_strdup (gtk_entry_get_text (GTK_ENTRY (priv->txt_decoded)));
}

/* PRIVATE */
static void
zak_gtk_decoder_decode (ZakGtkDecoder *decoder)
{
	ZakGtkDecoderPrivate *priv;

	g_return_if_fail (GTK_IS_FORM_DECODER (decoder));

	priv = ZAK_GTK_DECODER_GET_PRIVATE (decoder);

}

void
zak_gtk_decoder_show_btn_clean (ZakGtkDecoder *decoder, gboolean visible)
{
	ZakGtkDecoderPrivate *priv = ZAK_GTK_DECODER_GET_PRIVATE (decoder);

	gtk_entry_set_icon_from_stock (GTK_ENTRY (priv->txt_decoded), GTK_ENTRY_ICON_SECONDARY, visible ? "gtk-clear" : NULL);
	gtk_entry_set_icon_activatable (GTK_ENTRY (priv->txt_decoded), GTK_ENTRY_ICON_SECONDARY, visible);
	gtk_entry_set_icon_sensitive (GTK_ENTRY (priv->txt_decoded), GTK_ENTRY_ICON_SECONDARY, visible);
}

void
zak_gtk_decoder_show_btn_new (ZakGtkDecoder *decoder, gboolean visible)
{
	ZakGtkDecoderPrivate *priv = ZAK_GTK_DECODER_GET_PRIVATE (decoder);

	gtk_widget_set_visible (priv->btn_new, visible);
}

void
zak_gtk_decoder_show_btn_open (ZakGtkDecoder *decoder, gboolean visible)
{
	ZakGtkDecoderPrivate *priv = ZAK_GTK_DECODER_GET_PRIVATE (decoder);

	gtk_widget_set_visible (priv->btn_open, visible);
}

void
zak_gtk_decoder_show_btn_browse (ZakGtkDecoder *decoder, gboolean visible)
{
	ZakGtkDecoderPrivate *priv = ZAK_GTK_DECODER_GET_PRIVATE (decoder);

	gtk_widget_set_visible (priv->btn_browse, visible);
}

gboolean
zak_gtk_decoder_btn_clean_is_visible (ZakGtkDecoder *decoder)
{
	ZakGtkDecoderPrivate *priv = ZAK_GTK_DECODER_GET_PRIVATE (decoder);

	return gtk_entry_get_icon_stock (GTK_ENTRY (priv->txt_decoded), GTK_ENTRY_ICON_SECONDARY) != NULL;
}

gboolean
zak_gtk_decoder_btn_new_is_visible (ZakGtkDecoder *decoder)
{
	ZakGtkDecoderPrivate *priv = ZAK_GTK_DECODER_GET_PRIVATE (decoder);

	return gtk_widget_get_visible (priv->btn_new);
}

gboolean
zak_gtk_decoder_btn_open_is_visible (ZakGtkDecoder *decoder)
{
	ZakGtkDecoderPrivate *priv = ZAK_GTK_DECODER_GET_PRIVATE (decoder);

	return gtk_widget_get_visible (priv->btn_open);
}

gboolean
zak_gtk_decoder_btn_browse_is_visible (ZakGtkDecoder *decoder)
{
	ZakGtkDecoderPrivate *priv = ZAK_GTK_DECODER_GET_PRIVATE (decoder);

	return gtk_widget_get_visible (priv->btn_browse);
}

void
zak_gtk_decoder_set_editable (ZakGtkDecoder *decoder, gboolean editable)
{
	ZakGtkDecoderPrivate *priv = ZAK_GTK_DECODER_GET_PRIVATE (decoder);

	gtk_widget_set_sensitive (priv->btn_new, editable);
	gtk_widget_set_sensitive (priv->btn_open, editable);
	gtk_widget_set_sensitive (priv->btn_browse, editable);

	gtk_editable_set_editable (GTK_EDITABLE (priv->txt_decoded), editable);
}

gboolean
zak_gtk_decoder_get_editable (ZakGtkDecoder *decoder)
{
	ZakGtkDecoderPrivate *priv = ZAK_GTK_DECODER_GET_PRIVATE (decoder);

	return (gtk_widget_is_sensitive (priv->btn_new));
}

/* CALLBACKS */
static void
zak_gtk_decoder_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
	ZakGtkDecoder *decoder = ZAK_GTK_DECODER (object);
	ZakGtkDecoderPrivate *priv = ZAK_GTK_DECODER_GET_PRIVATE (decoder);

	switch (property_id)
		{
			case PROP_SHOW_CLEAN:
				zak_gtk_decoder_show_btn_clean (decoder, g_value_get_boolean (value));
				break;

			case PROP_SHOW_NEW:
				zak_gtk_decoder_show_btn_new (decoder, g_value_get_boolean (value));
				break;

			case PROP_SHOW_OPEN:
				zak_gtk_decoder_show_btn_open (decoder, g_value_get_boolean (value));
				break;

			case PROP_SHOW_BROWSE:
				zak_gtk_decoder_show_btn_browse (decoder, g_value_get_boolean (value));
				break;

			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_gtk_decoder_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
	ZakGtkDecoder *decoder = ZAK_GTK_DECODER (object);
	ZakGtkDecoderPrivate *priv = ZAK_GTK_DECODER_GET_PRIVATE (decoder);

	switch (property_id)
		{
			case PROP_SHOW_CLEAN:
				g_value_set_boolean (value, zak_gtk_decoder_btn_clean_is_visible (decoder));
				break;

			case PROP_SHOW_NEW:
				g_value_set_boolean (value, zak_gtk_decoder_btn_new_is_visible (decoder));
				break;

			case PROP_SHOW_OPEN:
				g_value_set_boolean (value, zak_gtk_decoder_btn_open_is_visible (decoder));
				break;

			case PROP_SHOW_BROWSE:
				g_value_set_boolean (value, zak_gtk_decoder_btn_browse_is_visible (decoder));
				break;

			default:
				G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
				break;
		}
}

static void
zak_gtk_decoder_size_request (GtkWidget *widget,
                        GtkRequisition *requisition)
{
	ZakGtkDecoder *decoder;
	GtkBin *bin;
	GtkRequisition child_requisition;

	guint border_width;

	g_return_if_fail (GTK_IS_FORM_DECODER (widget));
	g_return_if_fail (requisition != NULL);

	decoder = ZAK_GTK_DECODER (widget);
	bin = GTK_BIN (decoder);

	requisition->width = 0;
	requisition->height = 0;

	if (gtk_bin_get_child (bin) && gtk_widget_get_visible (GTK_WIDGET (gtk_bin_get_child (bin))))
		{
			gtk_widget_size_request (gtk_bin_get_child (bin), &child_requisition);
			requisition->width += child_requisition.width;
			requisition->height += child_requisition.height;
		}

	border_width = gtk_container_get_border_width (GTK_CONTAINER (widget));
	requisition->width += (border_width * 2);
	requisition->height += (border_width * 2);
}

static void
zak_gtk_decoder_size_allocate (GtkWidget *widget,
                         GtkAllocation *allocation)
{
	ZakGtkDecoder *decoder;
	GtkBin *bin;
	GtkAllocation w_allocation;
	GtkAllocation relative_allocation;
	GtkAllocation child_allocation;

	guint border_width;

	g_return_if_fail (GTK_IS_FORM_DECODER (widget));
	g_return_if_fail (allocation != NULL);

	decoder = ZAK_GTK_DECODER (widget);
	bin = GTK_BIN (decoder);

	gtk_widget_set_allocation (widget, allocation);

	border_width = gtk_container_get_border_width (GTK_CONTAINER (widget));
	relative_allocation.x = border_width;
	relative_allocation.y = border_width;
	gtk_widget_get_allocation (widget, &w_allocation);
	relative_allocation.width = MAX (1, w_allocation.width - relative_allocation.x * 2);
	relative_allocation.height = MAX (1, w_allocation.height - relative_allocation.y * 2);

	if (gtk_bin_get_child (bin) && gtk_widget_get_visible (GTK_WIDGET (gtk_bin_get_child (bin))))
		{
			child_allocation.x = relative_allocation.x + allocation->x;
			child_allocation.y = relative_allocation.y + allocation->y;
			child_allocation.width = relative_allocation.width;
			child_allocation.height = relative_allocation.height;
			gtk_widget_size_allocate (gtk_bin_get_child (bin), &child_allocation);
		}
}

static void
zak_gtk_decoder_on_btn_clean_clicked (GtkEntry *entry,
                                       GtkEntryIconPosition icon_pos,
                                       GdkEvent *event,
                                       gpointer user_data)
{
	gboolean ret;

	ZakGtkDecoder *decoder = (ZakGtkDecoder *)user_data;
	ZakGtkDecoderPrivate *priv = ZAK_GTK_DECODER_GET_PRIVATE (decoder);
	ZakGtkDecoderClass *klass = ZAK_GTK_DECODER_GET_CLASS (decoder);

	ret = FALSE;
	g_signal_emit (G_OBJECT (user_data), klass->btn_clean_clicked_signal_id, 0, 0, &ret);

	if (!ret)
		{
			zak_gtk_decoder_set_key (decoder, "0");
		}
}

static void
zak_gtk_decoder_on_btn_new_clicked (GtkButton *button,
                                        gpointer user_data)
{
	ZakGtkDecoderClass *klass = ZAK_GTK_DECODER_GET_CLASS ((ZakGtkDecoder *)user_data);

	g_signal_emit (G_OBJECT (user_data), klass->btn_new_clicked_signal_id, 0);
}

static void
zak_gtk_decoder_on_btn_open_clicked (GtkButton *button,
                                        gpointer user_data)
{
	ZakGtkDecoderClass *klass = ZAK_GTK_DECODER_GET_CLASS ((ZakGtkDecoder *)user_data);

	g_signal_emit (G_OBJECT (user_data), klass->btn_open_clicked_signal_id, 0);
}

static void
zak_gtk_decoder_on_btn_browse_clicked (GtkButton *button,
                                        gpointer user_data)
{
	ZakGtkDecoderClass *klass = ZAK_GTK_DECODER_GET_CLASS ((ZakGtkDecoder *)user_data);

	g_signal_emit (G_OBJECT (user_data), klass->btn_browse_clicked_signal_id, 0);
}
