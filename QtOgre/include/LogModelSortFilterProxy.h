#ifndef QTOGRE_LOGMODELSORTFILTERPROXY_H_
#define QTOGRE_LOGMODELSORTFILTERPROXY_H_

#include <QSortFilterProxyModel>

namespace QtOgre
{
	class LogModelSortFilterProxy : public QSortFilterProxyModel
	{
		Q_OBJECT
	public:
		LogModelSortFilterProxy(QObject *parent = 0);
		void setShowLineAndFile(bool show);
		void setVisisbleLevels(int level_bitmask) {
			mVisibleLevels = level_bitmask;
			invalidateFilter();
		}

	protected:
		bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
		bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const;

	private:
		bool mShowLineAndFile;
		int mVisibleLevels;
	};
}
#endif