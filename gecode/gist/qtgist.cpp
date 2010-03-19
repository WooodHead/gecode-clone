/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
 *  Main authors:
 *     Guido Tack <tack@gecode.org>
 *
 *  Copyright:
 *     Guido Tack, 2006
 *
 *  Last modified:
 *     $Date$ by $Author$
 *     $Revision$
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

  Gist::Gist(Space* root, bool bab, QWidget* parent,
             const Options& opt) : QWidget(parent) {
    QGridLayout* layout = new QGridLayout(this);

    QScrollArea* scrollArea = new QScrollArea(this);

    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setAlignment(Qt::AlignHCenter);
    scrollArea->setAutoFillBackground(true);
    QPalette myPalette(scrollArea->palette());
    myPalette.setColor(QPalette::Window, Qt::white);
    scrollArea->setPalette(myPalette);
    canvas = new TreeCanvas(root, bab, this,opt);
    canvas->setPalette(myPalette);
    canvas->setObjectName("canvas");

    scrollArea->setWidget(canvas);

    connect(canvas, SIGNAL(solution(const Space*)),
            this, SIGNAL(solution(const Space*)));

    connect(canvas, SIGNAL(finished(void)), this, SIGNAL(finished(void)));

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

    nodeStatInspector = new NodeStatInspector(this);
    connect(canvas, SIGNAL(statusChanged(VisualNode*,const Statistics&, bool)),
            nodeStatInspector, SLOT(node(VisualNode*,const Statistics&, bool)));

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

    bookmarkNode = new QAction("Add/remove bookmark", this);
    bookmarkNode->setShortcut(QKeySequence("Shift+B"));
    connect(bookmarkNode, SIGNAL(triggered()), canvas, SLOT(bookmarkNode()));

    connect(canvas, SIGNAL(addedBookmark(const QString&)),
            this, SLOT(addBookmark(const QString&)));
    connect(canvas, SIGNAL(removedBookmark(int)),
            this, SLOT(removeBookmark(int)));

    nullBookmark = new QAction("<none>",this);
    nullBookmark->setCheckable(true);
    nullBookmark->setChecked(false);
    nullBookmark->setEnabled(false);
    bookmarksGroup = new QActionGroup(this);
    bookmarksGroup->setExclusive(false);
    bookmarksGroup->addAction(nullBookmark);
    connect(bookmarksGroup, SIGNAL(triggered(QAction*)),
            this, SLOT(selectBookmark(QAction*)));

    bookmarksMenu = new QMenu("Bookmarks");
    connect(bookmarksMenu, SIGNAL(aboutToShow()),
            this, SLOT(populateBookmarksMenu()));

    setPath = new QAction("Set path", this);
    setPath->setShortcut(QKeySequence("Shift+P"));
    connect(setPath, SIGNAL(triggered()), canvas, SLOT(setPath()));

    inspectPath = new QAction("Inspect path", this);
    inspectPath->setShortcut(QKeySequence("Shift+I"));
    connect(inspectPath, SIGNAL(triggered()), canvas, SLOT(inspectPath()));

    showNodeStats = new QAction("Node statistics", this);
    showNodeStats->setShortcut(QKeySequence("S"));
    connect(showNodeStats, SIGNAL(triggered()),
            this, SLOT(showStats()));

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

    addAction(setPath);
    addAction(inspectPath);
    addAction(showNodeStats);

    nullSolutionInspector = new QAction("<none>",this);
    nullSolutionInspector->setCheckable(true);
    nullSolutionInspector->setChecked(false);
    nullSolutionInspector->setEnabled(false);
    solutionInspectorGroup = new QActionGroup(this);
    solutionInspectorGroup->setExclusive(false);
    solutionInspectorGroup->addAction(nullSolutionInspector);
    connect(solutionInspectorGroup, SIGNAL(triggered(QAction*)),
            this, SLOT(selectSolutionInspector(QAction*)));

    nullDoubleClickInspector = new QAction("<none>",this);
    nullDoubleClickInspector->setCheckable(true);
    nullDoubleClickInspector->setChecked(false);
    nullDoubleClickInspector->setEnabled(false);
    doubleClickInspectorGroup = new QActionGroup(this);
    doubleClickInspectorGroup->setExclusive(false);
    doubleClickInspectorGroup->addAction(nullDoubleClickInspector);
    connect(doubleClickInspectorGroup, SIGNAL(triggered(QAction*)),
            this, SLOT(selectDoubleClickInspector(QAction*)));

    nullMoveInspector = new QAction("<none>",this);
    nullMoveInspector->setCheckable(true);
    nullMoveInspector->setChecked(false);
    nullMoveInspector->setEnabled(false);
    moveInspectorGroup = new QActionGroup(this);
    moveInspectorGroup->setExclusive(false);
    moveInspectorGroup->addAction(nullMoveInspector);
    connect(moveInspectorGroup, SIGNAL(triggered(QAction*)),
            this, SLOT(selectMoveInspector(QAction*)));

    solutionInspectorMenu = new QMenu("Solution inspectors");
    solutionInspectorMenu->addActions(solutionInspectorGroup->actions());
    doubleClickInspectorMenu = new QMenu("Double click inspectors");
    doubleClickInspectorMenu->addActions(
      doubleClickInspectorGroup->actions());
    moveInspectorMenu = new QMenu("Move inspectors");
    moveInspectorMenu->addActions(moveInspectorGroup->actions());

    contextMenu = new QMenu(this);
    contextMenu->addAction(inspect);
    contextMenu->addAction(showNodeStats);
    contextMenu->addAction(center);

    contextMenu->addSeparator();

    contextMenu->addAction(searchNext);
    contextMenu->addAction(searchAll);

    contextMenu->addSeparator();

    contextMenu->addAction(toggleHidden);
    contextMenu->addAction(hideFailed);
    contextMenu->addAction(unhideAll);

    contextMenu->addSeparator();

    contextMenu->addMenu(bookmarksMenu);
    contextMenu->addAction(setPath);
    contextMenu->addAction(inspectPath);

    contextMenu->addSeparator();

    contextMenu->addMenu(doubleClickInspectorMenu);
    contextMenu->addMenu(solutionInspectorMenu);
    contextMenu->addMenu(moveInspectorMenu);

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
  Gist::addInspector(Inspector* i0, QAction*& nas, QAction*& nad,
                     QAction*&nam) {
    if (doubleClickInspectorGroup->
      actions().indexOf(nullDoubleClickInspector) != -1) {
      doubleClickInspectorGroup->removeAction(nullDoubleClickInspector);
      solutionInspectorGroup->removeAction(nullSolutionInspector);
      moveInspectorGroup->removeAction(nullMoveInspector);
    }
    canvas->addSolutionInspector(i0);
    canvas->addDoubleClickInspector(i0);
    canvas->addMoveInspector(i0);

    nas = new QAction(i0->name().c_str(), this);
    nas->setCheckable(true);
    solutionInspectorGroup->addAction(nas);
    solutionInspectorMenu->clear();
    solutionInspectorMenu->addActions(solutionInspectorGroup->actions());

    nad = new QAction(i0->name().c_str(), this);
    nad->setCheckable(true);
    doubleClickInspectorGroup->addAction(nad);
    doubleClickInspectorMenu->clear();
    doubleClickInspectorMenu->addActions(
      doubleClickInspectorGroup->actions());

    nam = new QAction(i0->name().c_str(), this);
    nam->setCheckable(true);
    moveInspectorGroup->addAction(nam);
    moveInspectorMenu->clear();
    moveInspectorMenu->addActions(
      moveInspectorGroup->actions());
  }

  void
  Gist::addSolutionInspector(Inspector* i) {
    QAction* nas;
    QAction* nad;
    QAction* nam;
    addInspector(i, nas,nad,nam);
    nas->setChecked(true);
    selectSolutionInspector(nas);
  }

  void
  Gist::addDoubleClickInspector(Inspector* i) {
    QAction* nas;
    QAction* nad;
    QAction* nam;
    addInspector(i, nas,nad,nam);
    nad->setChecked(true);
    selectDoubleClickInspector(nad);
  }

  void
  Gist::addMoveInspector(Inspector* i) {
    QAction* nas;
    QAction* nad;
    QAction* nam;
    addInspector(i, nas,nad,nam);
    nam->setChecked(true);
    selectMoveInspector(nam);
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
    } else {
      inspect->setEnabled(true);
      stop->setEnabled(false);
      reset->setEnabled(true);

      if ( (n->isOpen() || n->hasOpenChildren()) && (!n->isHidden()) ) {
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
    }
    emit statusChanged(stats,finished);
  }

  bool
  Gist::finish(void) {
    return canvas->finish();
  }

  void
  Gist::selectDoubleClickInspector(QAction* a) {
    canvas->activateDoubleClickInspector(
      doubleClickInspectorGroup->actions().indexOf(a),
      a->isChecked());
  }
  void
  Gist::selectSolutionInspector(QAction* a) {
    canvas->activateSolutionInspector(
      solutionInspectorGroup->actions().indexOf(a),
      a->isChecked());
  }
  void
  Gist::selectMoveInspector(QAction* a) {
    canvas->activateMoveInspector(
      moveInspectorGroup->actions().indexOf(a),
      a->isChecked());
  }
  void
  Gist::selectBookmark(QAction* a) {
    int idx = bookmarksGroup->actions().indexOf(a);
    canvas->setCurrentNode(canvas->bookmarks[idx]);
    canvas->centerCurrentNode();
  }

  void
  Gist::addBookmark(const QString& id) {
    if (bookmarksGroup->actions().indexOf(nullBookmark) != -1) {
      bookmarksGroup->removeAction(nullBookmark);
    }

    QAction* nb = new QAction(id, this);
    nb->setCheckable(true);
    bookmarksGroup->addAction(nb);
  }

  void
  Gist::removeBookmark(int idx) {
    QAction* a = bookmarksGroup->actions()[idx];
    bookmarksGroup->removeAction(a);
    if (bookmarksGroup->actions().size() == 0) {
      bookmarksGroup->addAction(nullBookmark);
    }
  }
  
  void
  Gist::populateBookmarksMenu(void) {
    bookmarksMenu->clear();
    bookmarksMenu->addAction(bookmarkNode);
    bookmarksMenu->addSeparator();
    bookmarksMenu->addActions(bookmarksGroup->actions());
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

  void
  Gist::showStats(void) {
    nodeStatInspector->showStats();
    canvas->emitStatusChanged();
  }

}}

// STATISTICS: gist-any
