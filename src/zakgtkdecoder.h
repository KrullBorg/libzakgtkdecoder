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

#ifndef __ZAK_GTK_DECODER_H__
#define __ZAK_GTK_DECODER_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS


#define GTK_TYPE_FORM_DECODER           (zak_gtk_decoder_get_type ())
#define ZAK_GTK_DECODER(obj)           (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_FORM_DECODER, ZakGtkDecoder))
#define ZAK_GTK_DECODER_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_FORM_DECODER, ZakGtkDecoderClass))
#define GTK_IS_FORM_DECODER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_FORM_DECODER))
#define GTK_IS_FORM_DECODER_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_FORM_DECODER))
#define ZAK_GTK_DECODER_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_FORM_DECODER, ZakGtkDecoderClass))


typedef struct _ZakGtkDecoder ZakGtkDecoder;
typedef struct _ZakGtkDecoderClass ZakGtkDecoderClass;


struct _ZakGtkDecoder
{
	GtkBin parent;
};

struct _ZakGtkDecoderClass
{
	GtkBinClass parent_class;

	guint btn_clean_clicked_signal_id;
	guint btn_new_clicked_signal_id;
	guint btn_open_clicked_signal_id;
	guint btn_browse_clicked_signal_id;
};


GType zak_gtk_decoder_get_type (void) G_GNUC_CONST;

GtkWidget *zak_gtk_decoder_new (void);

void zak_gtk_decoder_set_key (ZakGtkDecoder *decoder, const gchar *key);

gchar *zak_gtk_decoder_get_key (ZakGtkDecoder *decoder);
gchar *zak_gtk_decoder_get_decoded (ZakGtkDecoder *decoder);

void zak_gtk_decoder_show_btn_clean (ZakGtkDecoder *decoder, gboolean visible);
void zak_gtk_decoder_show_btn_new (ZakGtkDecoder *decoder, gboolean visible);
void zak_gtk_decoder_show_btn_open (ZakGtkDecoder *decoder, gboolean visible);
void zak_gtk_decoder_show_btn_browse (ZakGtkDecoder *decoder, gboolean visible);

gboolean zak_gtk_decoder_btn_clean_is_visible (ZakGtkDecoder *decoder);
gboolean zak_gtk_decoder_btn_new_is_visible (ZakGtkDecoder *decoder);
gboolean zak_gtk_decoder_btn_open_is_visible (ZakGtkDecoder *decoder);
gboolean zak_gtk_decoder_btn_browse_is_visible (ZakGtkDecoder *decoder);

void zak_gtk_decoder_set_editable (ZakGtkDecoder *decoder, gboolean editable);
gboolean zak_gtk_decoder_get_editable (ZakGtkDecoder *decoder);


G_END_DECLS

#endif /* __ZAK_GTK_DECODER_H__ */
