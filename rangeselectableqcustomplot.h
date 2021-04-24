#ifndef RANGESELECTABLEQCUSTOMPLOT_H
#define RANGESELECTABLEQCUSTOMPLOT_H

#include "interactableqcustomplot.h"

class RangeSelectableQCustomPlot : public InteractableQCustomPlot
{
    Q_OBJECT

public:
    RangeSelectableQCustomPlot(QWidget *parent = nullptr);

    void setFunction(const Function&);

    Function getSelectedRange();

    void plotFunction(const Function&, const QPen& = QPen(QBrush(QColor(Qt::black)), 1));

public slots:
    void setVisible(bool visible);

signals:
    void rangeSelectionStateChanged(bool);

private slots:
    void mouseMoveEvent(QMouseEvent *event);

    void mouseClickEvent(QMouseEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

private:
    QCPCurve *verticalLine, *start, *end;
    bool end_turn;
    double xAxis[2], yAxis[2];
    unsigned int start_index, end_index;

    void clearSelection();
};

#endif // RANGESELECTABLEQCUSTOMPLOT_H
