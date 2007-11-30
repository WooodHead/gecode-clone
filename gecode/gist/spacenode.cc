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

#include "gecode/gist/spacenode.hh"
#include <stack>

namespace Gecode { namespace Gist {
  
  enum BranchKind {
    NORMAL, SPECIAL_IN, SPECIAL_OUT
  };

  /// \brief Representation of a branch in the search tree
  class Branch {
  public:
    /// Alternative number
    int alternative;
    /// The best space known when the branch was created
    Space* ownBest;
    const BranchKind branchKind;
    union {
      const SpecialDesc* special;
    /// Branching description
      const BranchingDesc* branch;
    } desc;
    
    /// Constructor
    Branch(int a, const BranchingDesc* d, Space* best = NULL, BranchKind bk = NORMAL)
      : alternative(a), ownBest(best), branchKind(bk) {
        desc.branch = d;
      }
    Branch(int a, const SpecialDesc* d, BranchKind bk, Space* best = NULL)
      : alternative(a), ownBest(best), branchKind(bk) {
        desc.special = d;
      }
 };

  /// \brief Initial configuration settings for search
  class Config {
  public:
    /// Distance for adaptive recomputation
    static const int a_d = 100000;
    /// Distance for fixed recomputation
    static const int mrd = 1;
  };
  
  SpecialDesc::SpecialDesc(std::string varName, IntRelType r0, int v0)
    : vn(varName), r(r0), v(v0) {}
  
  BestSpace::BestSpace(Space* s0, Better* b0) : s(s0), b(b0) {}
  
  int
  SpaceNode::recompute(void) {
    int rdist = 0;
    
    if (workingSpace == NULL) {
      SpaceNode* curNode = this;
      SpaceNode* firstFixpoint = NULL;

      if(curNode->getStatus() != SPECIAL) {
        firstFixpoint = curNode;
      }
      
      std::stack<Branch> stck;
      bool specialNodeOnPath = false;
      
      while (curNode->copy == NULL) {
        SpaceNode* parent = static_cast<SpaceNode*>(curNode->getParent());

        if(curNode->getStatus() == SPECIAL) {
          Branch b(curNode->alternative, curNode->desc.special, SPECIAL_IN);
          stck.push(b);
          if(firstFixpoint == NULL && parent->getStatus() == BRANCH) {
            firstFixpoint = parent;
          }
          specialNodeOnPath = true;
        }
        else if(parent->getStatus() == SPECIAL) {
           Branch b(curNode->alternative, NULL, SPECIAL_OUT);
          stck.push(b);
        }
        else {
          Branch b(curNode->alternative, parent->desc.branch,
                   curBest == NULL ? NULL : curNode->ownBest);
          stck.push(b);
        }
        
        curNode = parent;
        rdist++;
      }
      Space* curSpace = curNode->copy->clone();
      
      Space* lastBest = NULL;
      SpaceNode* middleNode = curNode;
      int curDist = 0;

#ifdef GECODE_GIST_EXPERIMENTAL

      // TODO nikopp: as long as the SpecialDesc is evaluated in the switch below,
      //              we need this varMap here
      Reflection::VarMap vm;
      if(specialNodeOnPath)
        curSpace->getVars(vm);

#endif      

      while (!stck.empty()) {
        if (Config::a_d >= 0 &&
            curDist > Config::a_d &&
            middleNode->getStatus() != SPECIAL &&
            curDist == rdist / 2) {
          middleNode->copy = curSpace->clone();
        }
        Branch b = stck.top(); stck.pop();
        
        if(middleNode == firstFixpoint) {
          curSpace->status();
        }
        
        switch (b.branchKind) {
        case NORMAL:
            {
              curSpace->commit(b.desc.branch, b.alternative);
            }
            break;
        case SPECIAL_IN:
            {

#ifdef GECODE_GIST_EXPERIMENTAL

              if(b.desc.special != NULL) {

                const char* vName = b.desc.special->vn.c_str();

                IntVar iv = IntVar(Int::IntView(static_cast<Int::IntVarImp*> (vm.var(vName))));

                rel(curSpace, iv, b.desc.special->r, b.desc.special->v);
              }

#endif

            }
            break;
        case SPECIAL_OUT:
         // really do nothing
          break;
        }

        if (b.ownBest != NULL && b.ownBest != lastBest) {
          curBest->b->constrain(curSpace, b.ownBest);
          lastBest = b.ownBest;
        }
        curDist++;
        middleNode =
          static_cast<SpaceNode*>(middleNode->getChild(b.alternative));
      }
      workingSpace = curSpace;
      
    }
    return rdist;
  }
  
  Space*
  SpaceNode::donateSpace(int alt, Space* ownBest) {
    Space* ret = workingSpace;
    if (ret != NULL) {
      workingSpace = NULL;
      if (desc.branch != NULL)
        ret->commit(desc.branch, alt);
      
      if (ownBest != NULL) {
        curBest->b->constrain(ret, ownBest);
      }
    }
    return ret;
  }
  
  Space*
  SpaceNode::checkLAO(int alt, Space* ownBest) {
    Space* ret = NULL;
    if (copy != NULL && noOfOpenChildren == 1 && getParent() != NULL) {
      // last alternative optimization
      ret = copy;
      copy = NULL;
      
      if(desc.branch != NULL)
        ret->commit(desc.branch, alt);

      if (ownBest != NULL) {
        curBest->b->constrain(ret, ownBest);
      }
    }
    return ret;
  }
  
  void
  SpaceNode::acquireSpace(void) {
    SpaceNode* p = static_cast<SpaceNode*>(getParent());
    if (status == UNDETERMINED && curBest != NULL && ownBest == NULL &&
        p != NULL && curBest->s != p->ownBest) {
      ownBest = curBest->s;
    }
    if (workingSpace == NULL && p != NULL) {
      workingSpace = p->donateSpace(alternative, ownBest);
    }
    if (workingSpace == NULL) {
      if (recompute() > Config::mrd && Config::mrd >= 0 &&
          status != SPECIAL &&
          workingSpace->status() == SS_BRANCH) {
        copy = workingSpace->clone();  
      }
    }
    if (status != SPECIAL) {
      // always return a fixpoint
      workingSpace->status();
      if (copy == NULL && p != NULL && isOpen()) {
        copy = p->checkLAO(alternative, ownBest);
        if (copy != NULL && copy->status() == SS_FAILED) {
          delete copy;
          copy = NULL;
        }
      }
    }
  }
    
  void
  SpaceNode::solveUp(void) {
    SpaceNode* p = static_cast<SpaceNode*>(getParent());
    _hasSolvedChildren = true;
    if (p != NULL && !p->_hasSolvedChildren) {
        p->solveUp();
    }
  }
  
  void
  SpaceNode::closeChild(bool hadFailures, bool hadSolutions) {
    noOfOpenChildren--;
    _hasFailedChildren = _hasFailedChildren || hadFailures;
    _hasSolvedChildren = _hasSolvedChildren || hadSolutions;
    SpaceNode* p = static_cast<SpaceNode*>(getParent());
    if (noOfOpenChildren == 0) {
      // stats.close();
      if (p != NULL) {
        delete copy;
        copy = NULL;        
        p->closeChild(_hasFailedChildren, _hasSolvedChildren);
      }
    } else if (hadSolutions) {
        solveUp();
    }
  }

  SpaceNode::SpaceNode(int alt, BestSpace* cb)
  : copy(NULL), workingSpace(NULL), curBest(cb), ownBest(NULL) {
    desc.branch = NULL;
    alternative = alt;
    status = UNDETERMINED;
    _hasSolvedChildren = false;
    _hasFailedChildren = false;
  }

  SpaceNode::SpaceNode(Space* root, Better* b)
  : workingSpace(root), curBest(NULL), ownBest(NULL) {
    desc.branch = NULL;
    if (root == NULL) {
      status = FAILED;
      _hasSolvedChildren = false;
      _hasFailedChildren = true;
      setNumberOfChildren(0);
      copy = root;
      return;
    }
    if (!root->failed())
      copy = root->clone();
    else
      copy = root;
    status = UNDETERMINED;
    _hasSolvedChildren = false;
    _hasFailedChildren = false;
    if (b != NULL)
      curBest = new BestSpace(NULL, b);
  }

  SpaceNode::~SpaceNode(void) {
    if(status == SPECIAL)
      delete desc.special;
    else
      delete desc.branch;
    delete copy;
    delete workingSpace;
    if (getParent() == NULL)
      delete curBest;
  }
    
  Space*
  SpaceNode::getSpace(void) {
    acquireSpace();
    Space* ret = workingSpace;
    workingSpace = NULL;
    return ret;
  }
    
#ifdef GECODE_GIST_EXPERIMENTAL
  Space*
  SpaceNode::getInputSpace(void) {
    SpaceNode* p = static_cast<SpaceNode*>(getParent());
    
    Space* ret = p->getSpace();
    
    // TODO nikopp: do different things for special nodes
    ret->commit(p->desc.branch, alternative);
    
    return ret;
  }
#endif

  int
  SpaceNode::getNumberOfChildNodes(void) {
    int kids = getNumberOfChildren();
    if (kids == -1) {
      // stats.newDetermined();
      acquireSpace();
      switch (workingSpace->status()) {
      case SS_FAILED:
        {
          delete workingSpace;
          workingSpace = NULL;
          // we may have got a copy from LAO!
          if (getParent() != NULL) {
            delete copy;
            copy = NULL;
          }
          kids = 0;
          _hasSolvedChildren = false;
          _hasFailedChildren = true;
          status = FAILED;
          // stats.newFailure();
          // stats.newDepth(getDepth());
          SpaceNode* p = static_cast<SpaceNode*>(getParent());
          if (p != NULL)
            p->closeChild(true, false);
        }
        break;
      case SS_SOLVED:
        {
          kids = 0;
          status = SOLVED;
          // we may have got a copy from LAO
          if (getParent() != NULL) {
            delete copy;
            copy = NULL;
          }
          _hasSolvedChildren = true;
          _hasFailedChildren = false;
          // stats.newFailure();
          // stats.newDepth(getDepth());
          if (curBest != NULL) {
            curBest->s = workingSpace->clone();
          }
          SpaceNode* p = static_cast<SpaceNode*>(getParent());
          if (p != NULL)
            p->closeChild(false, true);
        }
        break;
      case SS_BRANCH:
        desc.branch = workingSpace->description();
        kids = desc.branch->alternatives();
        status = BRANCH;
        // stats.newChoice();
        // stats.newOpen();
        // stats.newUndetermined(kids);
        // stats.newDepth(getDepth() + 1);
        break;
      }
      changedStatus();
      setNumberOfChildren(kids);
      noOfOpenChildren = kids;
      for (int i=kids; i--;) {
        SpaceNode* child = createChild(i);
        setChild(i, child);
      }
    }
    return kids;
  }
  
  NodeStatus
  SpaceNode::getStatus(void) {
    return status;
  }
  
  void
  SpaceNode::setStatus(NodeStatus s) {
    status = s;
  }
    
  void
  SpaceNode::setSpecialDesc(const SpecialDesc* d) {
    desc.special = d;
  }
    
  int
  SpaceNode::getAlternative(void) {
    return alternative;
  }
    
  bool
  SpaceNode::isOpen(void) {
    return status == UNDETERMINED || noOfOpenChildren > 0;
  }
  
  void
  SpaceNode::openUp(void) {
    if(!isOpen() && parent != NULL)
      static_cast<SpaceNode*>(parent)->openUp();
    noOfOpenChildren++;
  }
  
  bool
  SpaceNode::hasFailedChildren(void) {
    return _hasFailedChildren;
  }
  
  bool
  SpaceNode::hasSolvedChildren(void) {
    return _hasSolvedChildren;
  }

  int
  SpaceNode::getNoOfOpenChildren(void) {
    return noOfOpenChildren;
  }
        
  void
  SpaceNode::setNoOfOpenChildren(int n) {
    assert(status == SPECIAL);
    noOfOpenChildren = n;
  }
        
  bool
  SpaceNode::hasCopy(void) {
    return copy != NULL;
  }

  bool
  SpaceNode::hasWorkingSpace(void) {
    return workingSpace != NULL;
  }

  SpaceNode*
  SpaceNode::createChild(int alternative) {
    return new SpaceNode(alternative, curBest);
  }
  
  void
  SpaceNode::changedStatus() {}
  
}}

// STATISTICS: gist-any
