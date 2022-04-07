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

#ifndef dooble_charts_iodevice_h
#define dooble_charts_iodevice_h

#include <QIODevice>
#include <QReadWriteLock>
#include <QTimer>
#include <QtDebug>

class dooble_charts_iodevice: public QIODevice
{
  Q_OBJECT

 public:
  dooble_charts_iodevice(QObject *parent, const int index):QIODevice(parent)
  {
#if (QT_VERSION < QT_VERSION_CHECK(5, 14, 0))
    m_finished.store(0);
    m_index.store(index);
#else
    m_finished.storeRelaxed(0);
    m_index.storeRelaxed(index);
#endif
    m_read_interval = 256;
    m_read_size = 1024;
    m_read_timer.setInterval(m_read_interval);
  }

  virtual ~dooble_charts_iodevice()
  {
    m_read_timer.stop();
  }

  virtual void pause(void)
  {
    m_read_timer.stop();
  }

  virtual void play(void)
  {
#if (QT_VERSION < QT_VERSION_CHECK(5, 14, 0))
    m_finished.store(0);
#else
    m_finished.storeRelaxed(0);
#endif
    m_read_timer.start();
  }

  virtual void rewind(void)
  {
  }

  virtual void stop(void)
  {
#if (QT_VERSION < QT_VERSION_CHECK(5, 14, 0))
    m_finished.store(1);
#else
    m_finished.storeRelaxed(1);
#endif
    m_read_timer.stop();
  }

  void set_address(const QString &address)
  {
    QWriteLocker locker(&m_address_mutex);

    m_address = address;
  }

  void set_data_extraction_script(const QString &program)
  {
    m_program = program.trimmed();
  }

  void set_read_interval(const int interval)
  {
    m_read_interval = qMax(1, interval);
    m_read_timer.setInterval(m_read_interval);
  }

  void set_read_rate(const QString &rate)
  {
    auto list(rate.split('/'));

    set_read_interval(list.value(1).trimmed().toInt());
    set_read_size(list.value(0).trimmed().toInt());
  }

  void set_read_size(const int size)
  {
#if (QT_VERSION < QT_VERSION_CHECK(5, 14, 0))
    m_read_size.store(qMax(1, size));
#else
    m_read_size.storeRelaxed(qMax(1, size));
#endif
  }

  void set_type(const QString &type)
  {
    m_type = type.trimmed();
  }

 protected:
  QAtomicInteger<short> m_finished;
  QAtomicInteger<int> m_index;
  QAtomicInteger<int> m_read_size;
  QReadWriteLock m_address_mutex;
  QString m_address;
  QString m_program;
  QString m_type;
  QTimer m_read_timer;
  int m_read_interval;

 signals:
  void data_ready(const QVector<double> &vector, const int index);
};

#endif
