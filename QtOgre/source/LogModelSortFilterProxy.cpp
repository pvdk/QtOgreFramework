#include "LogModelSortFilterProxy.h"

namespace QtOgre
{
	LogModelSortFilterProxy::LogModelSortFilterProxy(QObject *parent)
		:QSortFilterProxyModel(parent)
		,mShowLineAndFile(false) // TODO: add a GUI switch for this (right now nothing sends file/line info) so it's off
	{}

	bool LogModelSortFilterProxy::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const {
		// both of these must be true to show this row
		bool passed_level_test = false;
		bool passed_text_test = false;

		// go find the log level of the entry in the real model
		QModelIndex idx = sourceModel()->index(source_row, 0, source_parent);
		QVariant data = sourceModel()->data(idx, Qt::UserRole);

		// does it fit in our bitmask?
		if (data.toInt() & mVisibleLevels) {
			passed_level_test = true;
		}
		if (passed_level_test) // if it passed the bitmask, then make sure the text matches
			passed_text_test = QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
		return passed_level_test && passed_text_test; // has to match both
	}

	bool LogModelSortFilterProxy::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const {
		if (!mShowLineAndFile && source_column == 1 || source_column == 2) {
			return false;
		} else {
			return QSortFilterProxyModel::filterAcceptsColumn(source_column, source_parent);
		}
	}
}