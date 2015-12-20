/*
 * Copyright (C) 2015 Andrea Zagli <azagli@libero.it>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <gtk/gtk.h>

#include "zakgtkdecoder.h"

GtkWidget *w;
GtkWidget *decoder;

static gchar
*on_decode (gpointer instance, gchar *key, gpointer user_data)
{
	g_message ("Decoding.");
	return g_strdup_printf ("Decode for key «%s»", key);
}

static gboolean
on_btn_clean_clicked (gpointer instance, gpointer user_data)
{
	g_message ("Clean clicked.");
	return FALSE;
}

static gboolean
on_btn_new_clicked (gpointer instance, gpointer user_data)
{
	g_message ("New clicked.");
	return FALSE;
}

static gboolean
on_btn_open_clicked (gpointer instance, gpointer user_data)
{
	g_message ("Open clicked.");
	return FALSE;
}

static gboolean
on_btn_browse_clicked (gpointer instance, gpointer user_data)
{
	g_message ("Browse clicked.");
	zak_gtk_decoder_set_key (ZAK_GTK_DECODER (decoder), "1");
	return FALSE;
}

gboolean
on_w_delete_event (GtkWidget *widget,
                   GdkEvent *event,
                   gpointer user_data)
{
	return FALSE;
}

int
main (int argc, char **argv)
{
	GtkBuilder *builder;


	gtk_init (&argc, &argv);

	builder = gtk_builder_new ();

	gtk_builder_add_objects_from_file (builder, g_build_filename (GUIDIR, "decoder_xml.ui", NULL),
									   g_strsplit ("window1", "|" , -1),
									   NULL);

	w = GTK_WIDGET (gtk_builder_get_object (builder, "window1"));
	g_signal_connect (w, "delete-event", G_CALLBACK (on_w_delete_event), NULL);
	g_signal_connect (w, "destroy", gtk_main_quit, NULL);

	decoder = GTK_WIDGET (gtk_builder_get_object (builder, "zakgtkdecoder1"));

	g_signal_connect (G_OBJECT (decoder), "btn_clean_clicked",
	                  G_CALLBACK (on_btn_clean_clicked), NULL);
	g_signal_connect (G_OBJECT (decoder), "btn_new_clicked",
	                  G_CALLBACK (on_btn_new_clicked), NULL);
	g_signal_connect (G_OBJECT (decoder), "btn_open_clicked",
	                  G_CALLBACK (on_btn_open_clicked), NULL);
	g_signal_connect (G_OBJECT (decoder), "btn_browse_clicked",
	                  G_CALLBACK (on_btn_browse_clicked), NULL);

	g_signal_connect (G_OBJECT (decoder), "decode",
	                  G_CALLBACK (on_decode), NULL);

	gtk_widget_show_all (w);

	gtk_main ();

	return 0;
}
