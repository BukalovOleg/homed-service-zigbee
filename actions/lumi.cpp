#include <QtEndian>
#include "lumi.h"

QByteArray ActionsLUMI::PresenceSensor::request(const QString &name, const QVariant &data)
{
    switch (m_actions.indexOf(name))
    {
        case 0: // sensitivityMode
        {
            qint8 value = listIndex({"low", "medium", "high"}, data) + 1;
            m_attributes = {0x010C};
            return value < 1 ? QByteArray() : writeAttribute(DATA_TYPE_8BIT_UNSIGNED, &value, sizeof(value));
        }

        case 1: // detectionMode
        {
            qint8 value = listIndex({"undirected", "directed"}, data);
            m_attributes = {0x0144};
            return value < 0 ? QByteArray() : writeAttribute(DATA_TYPE_8BIT_UNSIGNED, &value, sizeof(value));
        }

        case 2: // distanceMode
        {
            qint8 value = listIndex({"far", "middle", "near"}, data);
            m_attributes = {0x0146};
            return value < 0 ? QByteArray() : writeAttribute(DATA_TYPE_8BIT_UNSIGNED, &value, sizeof(value));
        }

        case 3: // resetPresence
        {
            m_attributes.clear();
            return !data.toBool() ? QByteArray() : writeAttributeRequest(m_transactionId++, m_manufacturerCode, 0x0157, DATA_TYPE_8BIT_UNSIGNED, QByteArray(1, 0x01)); // TODO: check payload
        }
    }

    return QByteArray();
}

QByteArray ActionsLUMI::ButtonMode::request(const QString &name, const QVariant &data)
{
    QList <QString> list = {"relay", "leftRelay", "rightRelay", "decoupled"};
    qint8 value;

    switch (m_actions.indexOf(name))
    {
        case 2:  m_attributes = {0xFF23}; break; // rightMode
        default: m_attributes = {0xFF22}; break; // leftMode, buttonMode
    }

    switch (list.indexOf(data.toString()))
    {
        case 0:  value = 0x12; break;
        case 1:  value = 0x12; break;
        case 2:  value = 0x22; break;
        case 3:  value = 0xFE; break;
        default: return QByteArray();
    }

    return writeAttribute(DATA_TYPE_8BIT_UNSIGNED, &value, sizeof(value));
}

QByteArray ActionsLUMI::OperationMode::request(const QString &, const QVariant &data)
{
    qint8 value = listIndex({"command", "event"}, data);
    return value < 0 ? QByteArray() : writeAttribute(DATA_TYPE_8BIT_UNSIGNED, &value, sizeof(value));
}

QByteArray ActionsLUMI::IndicatorMode::request(const QString &, const QVariant &data)
{
    qint8 value = listIndex({"default", "inverted"}, data);
    return value < 0 ? QByteArray() : writeAttribute(DATA_TYPE_8BIT_UNSIGNED, &value, sizeof(value));
}

QByteArray ActionsLUMI::SwitchMode::request(const QString &, const QVariant &data)
{
    qint8 value = listIndex({"decoupled", "relay"}, data);
    return value < 0 ? QByteArray() : writeAttribute(DATA_TYPE_8BIT_UNSIGNED, &value, sizeof(value));
}

QByteArray ActionsLUMI::SwitchType::request(const QString &, const QVariant &data)
{
    qint8 value = listIndex({"toggle", "momentary"}, data) + 1;
    return value < 1 ? QByteArray() :writeAttribute(DATA_TYPE_8BIT_UNSIGNED, &value, sizeof(value));
}

QByteArray ActionsLUMI::SwitchStatusMemory::request(const QString &, const QVariant &data)
{
    quint8 value = data.toBool() ? 0x01 : 0x00;
    return writeAttribute(DATA_TYPE_BOOLEAN, &value, sizeof(value));
}

QByteArray ActionsLUMI::LightStatusMemory::request(const QString &, const QVariant &data)
{
    quint8 value = data.toBool() ? 0x01 : 0x00;
    return writeAttribute(DATA_TYPE_BOOLEAN, &value, sizeof(value));
}

QByteArray ActionsLUMI::Interlock::request(const QString &, const QVariant &data)
{
    quint8 value = data.toBool() ? 0x01 : 0x00;
    return writeAttribute(DATA_TYPE_BOOLEAN, &value, sizeof(value));
}

QByteArray ActionsLUMI::CoverPosition::request(const QString &, const QVariant &data)
{
    float value = data.toFloat();

    if (value > 100)
        value = 100;

    if (!option("invertCover").toBool())
        value = 100 - value;

    value = qToLittleEndian(value);
    return writeAttribute(DATA_TYPE_SINGLE_PRECISION, &value, sizeof(value));
}

QByteArray ActionsLUMI::VibrationSensitivity::request(const QString &, const QVariant &data)
{
    qint8 value = listIndex({"high", "medium", "low"}, data);

    if (value < 0)
        return QByteArray();

    value = value * 10 + 1;
    return writeAttribute(DATA_TYPE_8BIT_UNSIGNED, &value, sizeof(value));
}

QByteArray ActionsLUMI::SystemMode::request(const QString &, const QVariant &data)
{
    qint8 value = listIndex({"off", "heat"}, data);
    return value < 0 ? QByteArray() : writeAttribute(DATA_TYPE_8BIT_UNSIGNED, &value, sizeof(value));
}

QByteArray ActionsLUMI::ThermostatMode::request(const QString &, const QVariant &data)
{
    qint8 value = listIndex({"manual", "away", "program"}, data);
    return value < 0 ? QByteArray() : writeAttribute(DATA_TYPE_8BIT_UNSIGNED, &value, sizeof(value));
}

QByteArray ActionsLUMI::WindowDetection::request(const QString &, const QVariant &data)
{
    quint8 value = data.toBool() ? 0x01 : 0x00;
    return writeAttribute(DATA_TYPE_8BIT_UNSIGNED, &value, sizeof(value));
}

QByteArray ActionsLUMI::ChildLock::request(const QString &, const QVariant &data)
{
    quint8 value = data.toBool() ? 0x01 : 0x00;
    return writeAttribute(DATA_TYPE_8BIT_UNSIGNED, &value, sizeof(value));
}

QByteArray ActionsLUMI::AwayTemperature::request(const QString &, const QVariant &data)
{
    quint32 value = qToLittleEndian <quint32> (data.toDouble() * 100);
    return writeAttribute(DATA_TYPE_32BIT_UNSIGNED, &value, sizeof(value));
}
