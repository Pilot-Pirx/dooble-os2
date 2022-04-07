/*
** Copyright (c) 2008 - present, Alexis Megas.
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
** 3. The name of the author may not be used to endorse or promote products
**    derived from Dooble without specific prior written permission.
**
** DOOBLE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
** IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
** OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
** IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
** INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
** NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
** DOOBLE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef dooble_address_widget_h
#define dooble_address_widget_h

#include <QIcon>
#include <QLineEdit>
#include <QTextLayout>
#include <QUrl>

#include "dooble_history.h"

class QToolButton;
class dooble_address_widget_completer;
class dooble_web_engine_view;

class dooble_address_widget: public QLineEdit
{
  Q_OBJECT

 public:
  dooble_address_widget(QWidget *parent);
  QRect information_rectangle(void) const;
  QSize sizeHint(void) const;
  bool event(QEvent *event);
  void add_item(const QIcon &icon, const QUrl &url);
  void complete(void);
  void hide_popup(void);
  void prepare_containers_for_url(const QUrl &url);
  void setText(const QString &text);
  void set_item_icon(const QIcon &icon, const QUrl &url);
  void set_view(dooble_web_engine_view *view);

 protected:
  void dropEvent(QDropEvent *event);
  void keyPressEvent(QKeyEvent *event);
  void resizeEvent(QResizeEvent *event);

 private:
  QMenu *m_menu;
  QToolButton *m_favorite;
  QToolButton *m_information;
  QToolButton *m_pull_down;
  dooble_address_widget_completer *m_completer;
  dooble_web_engine_view *m_view;
  void prepare_icons(void);
  void set_text_format(const QList<QTextLayout::FormatRange> &formats);

 private slots:
  void slot_favorite(void);
  void slot_favorite_changed(const QUrl &url, bool state);
  void slot_favorites_cleared(void);
  void slot_favorites_populated(void);
  void slot_load_finished(bool ok);
  void slot_load_started(void);
  void slot_populate(const QListPairIconString &list);
  void slot_return_pressed(void);
  void slot_settings_applied(void);
  void slot_show_site_information_menu(void);
  void slot_text_edited(const QString &text);
  void slot_url_changed(const QUrl &url);
  void slot_zoom_reset(void);
  void slot_zoomed(qreal percent);

 signals:
  void favorite_changed(const QUrl &url, bool state);
  void inject_custom_css(void);
  void load_page(const QUrl &url);
  void populated(void);
  void pull_down_clicked(void);
  void show_certificate_exception(void);
  void show_site_cookies(void);
  void zoom_reset(void);
};

#endif
