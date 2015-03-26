#include "serial.h"

serial::serial(QSerialPort *serial, QObject *parent)
  : QObject(parent)
  , m_serialPort(serial)
  , m_standardOutput(stdout)
{
  connect(m_serialPort, SIGNAL(readyRead()), SLOT(handleReadyRead()));
  connect(m_serialPort, SIGNAL(error(QSerialPort::SerialPortError)), SLOT(handleError(QSerialPort::SerialPortError)));
  connect(&m_timer, SIGNAL(timeout()), SLOT(handleTimeout()));

  m_timer.start(5000);
}

serial::~serial()
{
}

void serial::handleReadyRead()
{
  m_readData.append(m_serialPort->readAll());

  if (!m_timer.isActive())
      m_timer.start(5000);
}

void serial::handleTimeout()
{
  if (m_readData.isEmpty()) {
      //m_standardOutput << QObject::tr("No data was currently available for reading from port %1").arg(m_serialPort->portName()) << endl;
  } else {
      m_standardOutput << QObject::tr("Data successfully received from port %1").arg(m_serialPort->portName()) << endl;
      m_standardOutput << m_readData << endl;
  }

  //exit(1);
}

void serial::handleError(QSerialPort::SerialPortError serialPortError)
{
  if (serialPortError == QSerialPort::ReadError) {
      m_standardOutput << QObject::tr("An I/O error occurred while reading the data from port %1, error: %2").arg(m_serialPort->portName()).arg(m_serialPort->errorString()) << endl;
      exit(1);
  }
}


