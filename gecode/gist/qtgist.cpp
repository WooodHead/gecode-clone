/*
 *  Main authors:
 *     Guido Tack <tack@gecode.org>
 *
 *  Copyright:
 *     Guido Tack, 2006
 *
 *  Last modified:
 *     $Date: 2008-09-03 14:14:11 +0200 (Mi, 03 Sep 2008) $ by $Author: tack $
 *     $Revision: 7787 $
 *
 *  This file is part of Gecode, the generic constraint
 *  development environment:
 *     http://www.gecode.org
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include <gecode/gist/qtgist.hh>

#include <gecode/gist/zoomToFitIcon.hpp>
#include <gecode/gist/nodevisitor.hh>
#include <gecode/gist/nodecursor.hh>

namespace Gecode { namespace Gist {

  Gist::Gist(Space* root, bool bab, QWidget* parent) : QWidget(parent) {
    QGridLayout* layout = new QGridLayout(this);    

    QScrollArea* scrollArea = new QScrollArea(this);
    
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setAlignment(Qt::AlignHCenter);
    scrollArea->setAutoFillBackground(true);
    QPalette myPalette(scrollArea->palette());
    myPalette.setColor(QPalette::Window, Qt::white);
    scrollArea->setPalette(myPalette);
    canvas = new TreeCanvas(root, bab, this);
    canvas->setPalette(myPalette);
    canvas->setObjectName("canvas");

    scrollArea->setWidget(canvas);

    QPixmap myPic;
    myPic.loadFromData(zoomToFitIcon, sizeof(zoomToFitIcon));

    QToolButton* autoZoomButton = new QToolButton();
    autoZoomButton->setCheckable(true);
    autoZoomButton->setIcon(myPic);

    QSlider* scaleBar = new QSlider(Qt::Vertical, this);
    canvas->scaleBar = scaleBar;
    scaleBar->setObjectName("scaleBar");
    scaleBar->setMinimum(LayoutConfig::minScale);
    scaleBar->setMaximum(LayoutConfig::maxScale);
    scaleBar->setValue(LayoutConfig::defScale);
    
    inspect = new QAction("Inspect", this);
    inspect->setShortcut(QKeySequence("Return"));
    connect(inspect, SIGNAL(triggered()), canvas, 
                       SLOT(inspectCurrentNode()));

    stop = new QAction("Stop search", this);
    stop->setShortcut(QKeySequence("Esc"));
    connect(stop, SIGNAL(triggered()), canvas, 
                    SLOT(stopSearch()));

    reset = new QAction("Reset", this);
    reset->setShortcut(QKeySequence("Ctrl+R"));
    connect(reset, SIGNAL(triggered()), canvas, 
            SLOT(reset()));

    navUp = new QAction("Up", this);
    navUp->setShortcut(QKeySequence("Up"));
    connect(navUp, SIGNAL(triggered()), canvas, 
                   SLOT(navUp()));

    navDown = new QAction("Down", this);
    navDown->setShortcut(QKeySequence("Down"));
    connect(navDown, SIGNAL(triggered()), canvas, 
                     SLOT(navDown()));

    navLeft = new QAction("Left", this);
    navLeft->setShortcut(QKeySequence("Left"));
    connect(navLeft, SIGNAL(triggered()), canvas, 
                     SLOT(navLeft()));

    navRight = new QAction("Right", this);
    navRight->setShortcut(QKeySequence("Right"));
    connect(navRight, SIGNAL(triggered()), canvas, 
                      SLOT(navRight()));

    navRoot = new QAction("Root", this);
    navRoot->setShortcut(QKeySequence("R"));
    connect(navRoot, SIGNAL(triggered()), canvas, 
                      SLOT(navRoot()));

    navNextSol = new QAction("To next solution", this);
    navNextSol->setShortcut(QKeySequence("Shift+Right"));
    connect(navNextSol, SIGNAL(triggered()), canvas, 
                      SLOT(navNextSol()));

    navPrevSol = new QAction("To previous solution", this);
    navPrevSol->setShortcut(QKeySequence("Shift+Left"));
    connect(navPrevSol, SIGNAL(triggered()), canvas, 
                      SLOT(navPrevSol()));

    searchNext = new QAction("Next solution", this);
    searchNext->setShortcut(QKeySequence("N"));
    connect(searchNext, SIGNAL(triggered()), canvas, SLOT(searchOne()));

    searchAll = new QAction("All solutions", this);
    searchAll->setShortcut(QKeySequence("A"));
    connect(searchAll, SIGNAL(triggered()), canvas, SLOT(searchAll()));

    toggleHidden = new QAction("Hide/unhide", this);
    toggleHidden->setShortcut(QKeySequence("H"));
    connect(toggleHidden, SIGNAL(triggered()), canvas, SLOT(toggleHidden()));

    hideFailed = new QAction("Hide failed subtrees", this);
    hideFailed->setShortcut(QKeySequence("F"));
    connect(hideFailed, SIGNAL(triggered()), canvas, SLOT(hideFailed()));

    unhideAll = new QAction("Unhide all", this);
    unhideAll->setShortcut(QKeySequence("U"));
    connect(unhideAll, SIGNAL(triggered()), canvas, SLOT(unhideAll()));

    zoomToFit = new QAction("Zoom to fit", this);
    zoomToFit->setShortcut(QKeySequence("Z"));
    connect(zoomToFit, SIGNAL(triggered()), canvas, SLOT(zoomToFit()));

    center = new QAction("Center current node", this);
    center->setShortcut(QKeySequence("C"));
    connect(center, SIGNAL(triggered()), canvas, SLOT(centerCurrentNode()));

    exportPDF = new QAction("Export subtree PDF...", this);
    exportPDF->setShortcut(QKeySequence("P"));
    connect(exportPDF, SIGNAL(triggered()), canvas, 
            SLOT(exportPDF()));

    exportWholeTreePDF = new QAction("Export PDF...", this);
    exportWholeTreePDF->setShortcut(QKeySequence("Ctrl+Shift+P"));
    connect(exportWholeTreePDF, SIGNAL(triggered()), canvas, 
            SLOT(exportWholeTreePDF()));

    print = new QAction("Print...", this);
    print->setShortcut(QKeySequence("Ctrl+P"));
    connect(print, SIGNAL(triggered()), canvas, 
            SLOT(print()));

    setPath = new QAction("Set path", this);
    setPath->setShortcut(QKeySequence("Shift+P"));
    connect(setPath, SIGNAL(triggered()), canvas, SLOT(setPath()));

    inspectPath = new QAction("Inspect path", this);
    inspectPath->setShortcut(QKeySequence("Shift+I"));
    connect(inspectPath, SIGNAL(triggered()), canvas, SLOT(inspectPath()));

    addVisualisation = new QAction("Add visualisation", this);
    addVisualisation->setShortcut(QKeySequence("Shift+V"));
    connect(addVisualisation, SIGNAL(triggered()), canvas, SLOT(addVisualisation()));

    addAction(inspect);
    addAction(stop);
    addAction(reset);
    addAction(navUp);
    addAction(navDown);
    addAction(navLeft);
    addAction(navRight);
    addAction(navRoot);
    addAction(navNextSol);
    addAction(navPrevSol);

    addAction(searchNext);
    addAction(searchAll);
    addAction(toggleHidden);
    addAction(hideFailed);
    addAction(unhideAll);
    addAction(zoomToFit);
    addAction(center);
    addAction(exportPDF);
    addAction(exportWholeTreePDF);
    addAction(print);

    addAction(addVisualisation);
    
    addAction(setPath);
    addAction(inspectPath);

    nullInspector = new QAction("<none>",this);
    inspectorGroup = new QActionGroup(this);
    inspectorGroup->setExclusive(true);
    inspectorGroup->addAction(nullInspector);
    connect(inspectorGroup, SIGNAL(triggered(QAction*)),
            this, SLOT(selectInspector(QAction*)));

    inspectorMenu = new QMenu("Inspectors");
    inspectorMenu->addActions(inspectorGroup->actions());

    contextMenu = new QMenu(this);
    contextMenu->addAction(inspect);
    contextMenu->addAction(center);

    contextMenu->addSeparator();

    contextMenu->addAction(searchNext);
    contextMenu->addAction(searchAll);      

    contextMenu->addSeparator();

    contextMenu->addAction(toggleHidden);
    contextMenu->addAction(hideFailed);
    contextMenu->addAction(unhideAll);

    contextMenu->addSeparator();

    contextMenu->addAction(setPath);
    contextMenu->addAction(inspectPath);

    contextMenu->addSeparator();
    
    contextMenu->addMenu(inspectorMenu);

    connect(scaleBar, SIGNAL(valueChanged(int)), canvas, SLOT(scaleTree(int)));

    connect(canvas, SIGNAL(scaleChanged(int)), scaleBar, SLOT(setValue(int)));

    connect(autoZoomButton, SIGNAL(toggled(bool)), canvas,
            SLOT(setAutoZoom(bool)));

    connect(canvas, SIGNAL(autoZoomChanged(bool)),
            autoZoomButton, SLOT(setChecked(bool)));

    connect(&canvas->searcher, SIGNAL(scaleChanged(int)),
            scaleBar, SLOT(setValue(int)));
    
    layout->addWidget(scrollArea, 0,0,-1,1);
    layout->addWidget(scaleBar, 1,1, Qt::AlignHCenter);
    layout->addWidget(autoZoomButton, 0,1, Qt::AlignHCenter);
    
    setLayout(layout);

    canvas->show();

    resize(500, 400);

    // enables on_<sender>_<signal>() mechanism
    QMetaObject::connectSlotsByName(this);
  }

  void
  Gist::resizeEvent(QResizeEvent*) {
    canvas->resizeToOuter();
  }

  void
  Gist::addInspector(Inspector* i0) {
    canvas->addInspector(i0);
    QAction* na = new QAction(i0->name().c_str(), this);
    na->setCheckable(true);
    inspectorGroup->addAction(na);
    inspectorMenu->clear();
    if (nullInspector->actionGroup() == inspectorGroup) {
      inspectorGroup->removeAction(nullInspector);
      na->setChecked(true);
    }
    inspectorMenu->addActions(inspectorGroup->actions());
  }
  
  Gist::~Gist(void) { delete canvas; }
  
  void
  Gist::on_canvas_contextMenu(QContextMenuEvent* event) {
    contextMenu->popup(event->globalPos());    
  }

  void
  Gist::on_canvas_statusChanged(VisualNode* n, const Statistics& stats,
                                bool finished) {
    if (!finished) {
      inspect->setEnabled(false);
      stop->setEnabled(true);
      reset->setEnabled(false);
      navUp->setEnabled(false);
      navDown->setEnabled(false);
      navLeft->setEnabled(false);
      navRight->setEnabled(false);
      navRoot->setEnabled(false);
      navNextSol->setEnabled(false);
      navPrevSol->setEnabled(false);

      searchNext->setEnabled(false);
      searchAll->setEnabled(false);
      toggleHidden->setEnabled(false);
      hideFailed->setEnabled(false);
      unhideAll->setEnabled(false);
      zoomToFit->setEnabled(false);
      center->setEnabled(false);
      exportPDF->setEnabled(false);
      exportWholeTreePDF->setEnabled(false);
      print->setEnabled(false);

      setPath->setEnabled(false);
      inspectPath->setEnabled(false);
      addVisualisation->setEnabled(false);
    } else {
      inspect->setEnabled(true);
      stop->setEnabled(false);
      reset->setEnabled(true);

      if (n->isOpen() || n->hasOpenChildren()) {
        searchNext->setEnabled(true);
        searchAll->setEnabled(true);
      } else {
        searchNext->setEnabled(false);
        searchAll->setEnabled(false);      
      }
      if (n->getNumberOfChildren() > 0) {
        navDown->setEnabled(true);
        toggleHidden->setEnabled(true);
        hideFailed->setEnabled(true);
        unhideAll->setEnabled(true);            
      } else {
        navDown->setEnabled(false);
        toggleHidden->setEnabled(false);
        hideFailed->setEnabled(false);
        unhideAll->setEnabled(false);      
      }

      VisualNode* p = n->getParent();
      if (p == NULL) {
        navRoot->setEnabled(false);
        navUp->setEnabled(false);
        navRight->setEnabled(false);
        navLeft->setEnabled(false);
      } else {
        navRoot->setEnabled(true);
        navUp->setEnabled(true);
        unsigned int alt = n->getAlternative();
        navRight->setEnabled(alt + 1 < p->getNumberOfChildren());
        navLeft->setEnabled(alt > 0);
      }

      VisualNode* root = n;
      while (!root->isRoot())
        root = root->getParent();
      NextSolCursor nsc(n, false);
      PreorderNodeVisitor<NextSolCursor> nsv(nsc);
      while (nsv.next()) {}
      navNextSol->setEnabled(nsv.getCursor().node() != root);

      NextSolCursor psc(n, true);
      PreorderNodeVisitor<NextSolCursor> psv(psc);
      while (psv.next()) {}
      navPrevSol->setEnabled(psv.getCursor().node() != root);

      zoomToFit->setEnabled(true);
      center->setEnabled(true);
      exportPDF->setEnabled(true);
      exportWholeTreePDF->setEnabled(true);
      print->setEnabled(true);

      setPath->setEnabled(true);
      inspectPath->setEnabled(true);
      addVisualisation->setEnabled(true);
    }
    emit statusChanged(stats,finished);
  }
  
  void
  Gist::finish(void) {
    canvas->finish();
  }

  void
  Gist::closeEvent(QCloseEvent* event) {
    canvas->finish();
    event->accept();
  }

  void
  Gist::selectInspector(QAction* a) {
    if (a != nullInspector)
      canvas->setActiveInspector(inspectorGroup->actions().indexOf(a));
  }

  void
  Gist::setAutoHideFailed(bool b) { canvas->setAutoHideFailed(b); }
  void
  Gist::setAutoZoom(bool b) { canvas->setAutoZoom(b); }
  bool
  Gist::getAutoHideFailed(void) { return canvas->getAutoHideFailed(); }
  bool
  Gist::getAutoZoom(void) { return canvas->getAutoZoom(); }
  void
  Gist::setRefresh(int i) { canvas->setRefresh(i); }
  bool
  Gist::getSmoothScrollAndZoom(void) {
    return canvas->getSmoothScrollAndZoom();
  }
  void
  Gist::setSmoothScrollAndZoom(bool b) {
    canvas->setSmoothScrollAndZoom(b);
  }
  void
  Gist::setRecompDistances(int c_d, int a_d) {
    canvas->setRecompDistances(c_d, a_d);
  }

  int
  Gist::getCd(void) {
    return canvas->c_d;
  }
  int
  Gist::getAd(void) {
    return canvas->a_d;
  }

  void
  Gist::setShowCopies(bool b) {
    canvas->setShowCopies(b);
  }
  bool
  Gist::getShowCopies(void) {
    return canvas->getShowCopies();
  }

}}

// STATISTICS: gist-any