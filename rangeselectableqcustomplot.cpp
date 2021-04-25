#include "rangeselectableqcustomplot.h"
#include "ui_interactableqcustomplot.h"

RangeSelectableQCustomPlot::RangeSelectableQCustomPlot(QWidget *parent) :
    InteractableQCustomPlot(parent)
{
    start = nullptr;
    start_index = 0;
    end = nullptr;
    end_index = 0;
    end_turn = false;
    verticalLine = new QCPCurve(ui->plot_widget->xAxis, ui->plot_widget->yAxis);
    ui->plot_widget->addPlottable(verticalLine);
    verticalLine->setName("Vertical");
    verticalLine->setPen(QPen(QBrush(QColor(Qt::black)), 1));
}

void RangeSelectableQCustomPlot::setFunction(const Function &function_input) {
    if (function_input == Function()) {
        return;
    }
    clearSelection();

    InteractableQCustomPlot::setFunction(function_input);

    delete verticalLine;
    verticalLine = new QCPCurve(ui->plot_widget->xAxis, ui->plot_widget->yAxis);
    ui->plot_widget->addPlottable(verticalLine);
    verticalLine->setName("Vertical");
    verticalLine->setPen(QPen(QBrush(QColor(Qt::black)), 1));
}

Function RangeSelectableQCustomPlot::getSelectedRange() {
    Function data;
    for (unsigned int i = start_index; i <= end_index; i++) {
        data.add(function.getElement(i));
    }
    return data;
}

void RangeSelectableQCustomPlot::plotFunction(const Function &data, const QPen &pen) {
    if (data == Function()) {
        return;
    }

    double max = -INFINITY, min = INFINITY;
    for (unsigned int i = 0; i < data.size(); i++) {
        if (max < data.getValue(i)) {
            max = data.getValue(i);
        }
        if (min > data.getValue(i)) {
            min = data.getValue(i);
        }
    }

    bool max_min_changed = false;
    if (max > max_y) {
        max_y = max;
        max_min_changed = true;
    }
    if (min < min_y) {
        min_y = min;
        max_min_changed = true;
    }
    if (max_min_changed) {
        if (start->visible()) {
            QVector<double> x_temp(2), y_temp(2);
            x_temp[0] = x_temp[1] = function.getKey(start_index);
            y_temp[0] = min_y - 3;
            y_temp[1] = max_y + 3;
            start->setData(x_temp, y_temp);
            ui->plot_widget->replot();
        }
        if (end->visible()) {
            QVector<double> x_temp(2), y_temp(2);
            x_temp[0] = x_temp[1] = function.getKey(end_index);
            y_temp[0] = min_y - 3;
            y_temp[1] = max_y + 3;
            end->setData(x_temp, y_temp);
            ui->plot_widget->replot();
        }
    }

    ui->plot_widget->addGraph();
    ui->plot_widget->graph(ui->plot_widget->graphCount() - 1)->setPen(pen);
    ui->plot_widget->graph(ui->plot_widget->graphCount() - 1)->setData(data);
    ui->plot_widget->replot();
}

void RangeSelectableQCustomPlot::setVisible(bool visible) {
    if (visible && !isVisible()) {
        connect(ui->plot_widget, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(mouseMoveEvent(QMouseEvent*)));
        connect(ui->plot_widget, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePressEvent(QMouseEvent*)));
        connect(ui->plot_widget, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(mouseReleaseEvent(QMouseEvent*)));
    } else if (!visible && isVisible()) {
        disconnect(ui->plot_widget, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(mouseMoveEvent(QMouseEvent*)));
        disconnect(ui->plot_widget, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePressEvent(QMouseEvent*)));
        disconnect(ui->plot_widget, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(mouseReleaseEvent(QMouseEvent*)));
        clearSelection();
    }
    InteractableQCustomPlot::setVisible(visible);
}

void RangeSelectableQCustomPlot::mouseMoveEvent(QMouseEvent *event)
{
    if (verticalLine == nullptr) {
        return;
    }
    double ystart = min_y - 3;
    double yend = max_y + 3;
    double key = ui->plot_widget->xAxis->pixelToCoord(event->pos().x());

    QVector<double> x(2), y(2);
    x[0] = x[1] = key;
    y[0] = ystart;
    y[1] = yend;

    verticalLine->setData(x, y);
    ui->plot_widget->replot();
}

void RangeSelectableQCustomPlot::mousePressEvent(QMouseEvent *event)
{
    if (verticalLine == nullptr) {
        return;
    }
    if (event->button() == Qt::LeftButton) {
        xAxis[0] = ui->plot_widget->xAxis->range().lower;
        xAxis[1] = ui->plot_widget->xAxis->range().upper;
        yAxis[0] = ui->plot_widget->yAxis->range().lower;
        yAxis[1] = ui->plot_widget->yAxis->range().upper;
    }
}

void RangeSelectableQCustomPlot::mouseReleaseEvent(QMouseEvent *event)
{
    if (verticalLine == nullptr) {
        return;
    }
    if (event->button() == Qt::LeftButton) {
        if (xAxis[0] == ui->plot_widget->xAxis->range().lower &&
            xAxis[1] == ui->plot_widget->xAxis->range().upper &&
            yAxis[0] == ui->plot_widget->yAxis->range().lower &&
            yAxis[1] == ui->plot_widget->yAxis->range().upper) {
            mouseClickEvent(event);
        }
    }
}

void RangeSelectableQCustomPlot::mouseClickEvent(QMouseEvent *event)
{
    if (verticalLine == nullptr) {
        return;
    }
    double ystart = min_y - 3;
    double yend = max_y + 3;
    double key = ui->plot_widget->xAxis->pixelToCoord(event->pos().x());

    unsigned int i;
    for (i = 0; i < function.size(); i++) {
        if (function.getKey(i) >= key) {
            break;
        }
    }
    if (i != 0) {
        if (abs(key - function.getKey(i)) > abs(function.getKey(i - 1) - key)) {
            i--;
        }
    }
    key = function.getKey(i);

    QVector<double> x(2), y(2);
    x[0] = x[1] = key;
    y[0] = ystart;
    y[1] = yend;

    if (start == nullptr) {
        start = new QCPCurve(ui->plot_widget->xAxis, ui->plot_widget->yAxis);
        ui->plot_widget->addPlottable(start);
        start->setName("Start");
        start->setPen(QPen(QBrush(QColor(Qt::red)), 1));
        start->setData(x, y);
        start_index = i;
        end_turn = true;
        ui->plot_widget->replot();
        return;
    } else if (end == nullptr) {
        end = new QCPCurve(ui->plot_widget->xAxis, ui->plot_widget->yAxis);
        ui->plot_widget->addPlottable(end);
        end->setName("End");
        end->setPen(QPen(QBrush(QColor(Qt::red)), 1));
        end->setData(x, y);
        end_index = i;
        if (key < function.getKey(start_index)) {
            start->setVisible(false);
        }
        end_turn = false;
    } else if (end_turn) {
        end->setData(x, y);
        end_index = i;
        if (key < function.getKey(start_index)) {
            start->setVisible(false);
        }
        end_turn = false;
        end->setVisible(true);
    } else {
        start->setData(x, y);
        start_index = i;
        if (key > function.getKey(end_index)) {
            end->setVisible(false);
        }
        end_turn = true;
        start->setVisible(true);
    }
    ui->plot_widget->replot();

    emit rangeSelectionStateChanged((start->visible()) && (end->visible()));
}

void RangeSelectableQCustomPlot::clearSelection() {
    bool has_been_cleaned = false;
    if (start != nullptr) {
        ui->plot_widget->removePlottable(start);
        //delete start;
        start = nullptr;
        has_been_cleaned = true;
    }
    if (end != nullptr) {
        ui->plot_widget->removePlottable(end);
        //delete end;
        end = nullptr;
        has_been_cleaned = true;
    }
    if (has_been_cleaned) {
        emit rangeSelectionStateChanged(false);
        start_index = 0;
        end_index = 0;
    }
}
