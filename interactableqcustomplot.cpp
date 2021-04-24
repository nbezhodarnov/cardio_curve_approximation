#include "interactableqcustomplot.h"
#include "ui_interactableqcustomplot.h"

InteractableQCustomPlot::InteractableQCustomPlot(QWidget *parent, const Function& function_input) :
    QWidget(parent),
    ui(new Ui::InteractableQCustomPlot),
    min_y(0),
    max_y(1)
{
    ui->setupUi(this);

    InteractableQCustomPlot::setFunction(function_input);

    ui->plot_widget->setInteractions(QCP::iRangeZoom | QCP::iRangeDrag);
    ui->plot_widget->xAxis->setLabel("x");
    ui->plot_widget->yAxis->setLabel("y");


    /*ui->plot_widget->setVisible(true);
    ui->plot_widget->setEnabled(true);*/
    if (isVisible()) {
        ui->plot_widget->setMouseTracking(true);
        connect(ui->plot_widget->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(plotrender_xaxis_range_changed(QCPRange)));
        connect(ui->plot_widget->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(plotrender_yaxis_range_changed(QCPRange)));
    }
}

InteractableQCustomPlot::~InteractableQCustomPlot()
{
    delete ui;
}

void InteractableQCustomPlot::setFunction(const Function &function_input) {
    if (function_input.size() < 1) {
        return;
    }

    function = function_input;

    max_y = function.getValue(0);
    min_y = function.getValue(0);
    for (unsigned int i = 1; i < function.size(); i++) {
        if (function.getValue(i) < min_y) {
            min_y = function.getValue(i);
        }
        if (function.getValue(i) > max_y) {
            max_y = function.getValue(i);
        }
    }

    ui->plot_widget->clearGraphs();
    ui->plot_widget->addGraph();
    ui->plot_widget->graph(0)->setData(function);
    ui->plot_widget->graph(0)->setName("Function");
    ui->plot_widget->xAxis->setRange(function.getKey(0), function.getKey(function.size() - 1));
    ui->plot_widget->yAxis->setRange(min_y, max_y);
    ui->plot_widget->replot();
}

double InteractableQCustomPlot::getMinimum() {
    return min_y;
}

double InteractableQCustomPlot::getMaximum() {
    return max_y;
}

void InteractableQCustomPlot::setVisible(bool visible) {
    if (visible && !isVisible()) {
        ui->plot_widget->setMouseTracking(true);
        connect(ui->plot_widget->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(plotrender_xaxis_range_changed(QCPRange)));
        connect(ui->plot_widget->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(plotrender_yaxis_range_changed(QCPRange)));
    } else if (!visible && isVisible()) {
        ui->plot_widget->setMouseTracking(false);
        disconnect(ui->plot_widget->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(plotrender_xaxis_range_changed(const QCPRange &)));
        disconnect(ui->plot_widget->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(plotrender_yaxis_range_changed(const QCPRange &)));
    }
    QWidget::setVisible(visible);
}

void InteractableQCustomPlot::plotrender_xaxis_range_changed(QCPRange newRange)
{
    double lower = newRange.lower, upper = newRange.upper;
    if (lower < function.getKey(0)) {
        lower = function.getKey(0);
    }
    if (upper > function.getKey(function.size() - 1)) {
        upper = function.getKey(function.size() - 1);
    }
    ui->plot_widget->xAxis->setRange(lower, upper);
}

void InteractableQCustomPlot::plotrender_yaxis_range_changed(QCPRange newRange)
{
    double lower = newRange.lower, upper = newRange.upper;
    if (lower < min_y - 3) {
        lower = min_y - 3;
    }
    if (upper > max_y + 3) {
        upper = max_y + 3;
    }
    ui->plot_widget->yAxis->setRange(lower, upper);
}
