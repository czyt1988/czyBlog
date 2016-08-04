#include "QVariantListListModel.h"

QVariantListListModel::QVariantListListModel(QObject *parent)
    : QAbstractTableModel(parent)
    ,m_varListList(NULL)
{
}

QVariant QVariantListListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(Qt::DisplayRole == role)
    {
        if(Qt::Horizontal == orientation)
        {
            return section + 1;
        }
        return section + 1;
    }
    return QVariant();
}

int QVariantListListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_rowCount;
    // FIXME: Implement me!
}

int QVariantListListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_columnCount;
    // FIXME: Implement me!
}

QVariant QVariantListListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if(NULL == m_varListList)
        return QVariant();
    if(Qt::DisplayRole == role)
    {
        const int row = index.row();
        const int col = index.column();
        if(row >= m_varListList->size())
            return QVariant();
        const int colSize = m_varListList->at(row).size();
        if(col >= colSize)
            return QVariant();
        return m_varListList->at(row).at(col);
    }
    return QVariant();
}

void QVariantListListModel::setVariantListListPtr(QList<QList<QVariant> > *ptr)
{
    if(NULL == ptr)
        return;
    m_varListList = ptr;
    updateData();
}

void QVariantListListModel::updateData()
{
    beginResetModel();
    m_rowCount = m_varListList->size();
    m_columnCount = 0;
    int colSize=0;
    for(int i=0;i<m_rowCount;++i)
    {
        colSize = m_varListList->at(i).size();
        if(colSize > m_columnCount)
        {
            m_columnCount = colSize;
        }
    }
    endResetModel();
}
