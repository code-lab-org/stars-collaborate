// Copyright (C) 2019 Ryan Linnabary
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "collaborate/tree.h"

#include <cstdint>
#include <fstream>
#include <queue>
#include <stack>
#include <string>
#include <vector>

#include "collaborate/node.h"

namespace osse {
namespace collaborate {

Tree::Tree(Node* _root_identity, const uint16_t& _height, Node* _target)
    : root_(CreateRoot(_root_identity)),
      size_(1),
      height_(_height),
      target_(_target) {
}

Tree::~Tree() {
  DeleteSubtree(root_);
  delete root_;
}

void Tree::DeleteSubtree(Tree::Branch* _branch) {
  if (!IsLeaf(_branch)) {
    while (!_branch->children.empty()) {
      Tree::Branch* child = _branch->children.front();
      DeleteSubtree(child);
      _branch->children.erase(_branch->children.begin());
    }
  }
  if (!(_branch->parent == nullptr)) {
    delete _branch;
  }
}

Tree::Branch* Tree::CreateRoot(Node* _root_identity) {
  Branch* root = new Branch;
  root->parent = nullptr;
  root->level = 0;
  root->identity = _root_identity;
  root->rx_time_s = 0;
  return root;
}

Tree::Branch* Tree::AddChild(Branch* _parent,
                             Node* _identity,
                             const uint64_t& _rx_time_s) {
  Branch* child = nullptr;
  bool has_child = HasChild(_parent, _identity);
  bool too_tall = _parent->level >= height_;
  bool is_target = (target_ == _identity);
  if ((!has_child && !too_tall) || is_target) {
    child = new Branch;
    child->parent = _parent;
    child->level = _parent->level + 1;
    child->identity = _identity;
    child->rx_time_s = _rx_time_s;
    _parent->children.insert(std::begin(_parent->children), child);
    size_++;
  }
  return child;
}

std::vector<Tree::Branch*> Tree::Ancestry(Tree::Branch* _branch) const {
  std::vector<Tree::Branch*> ancestry;
  Branch* branch = _branch;
  while (branch != nullptr) {
    ancestry.insert(ancestry.begin(), branch);
    branch = branch->parent;
  }
  return ancestry;
}

Tree::Branch* Tree::SearchSpecific(Node* _identity,
                                   const uint64_t& _rx_time_s) const {
  Branch* branch_ = nullptr;
  bool found = false;
  std::queue<Tree::Branch*> queue;
  queue.push(root_);
  while (!queue.empty() && !found) {
    Branch* branch = queue.front();
    bool same_node = branch->identity == _identity;
    bool same_rx_time = branch->rx_time_s == _rx_time_s;
    if (same_node && same_rx_time) {
      found = true;
      branch_ = branch;
    }
    if (!found) {
      queue.pop();
      for (auto child : branch->children) {
        queue.push(child);
      }
    }
  }
  return branch_;
}

Tree::Branch* Tree::BreadthFirstSearch(Node* _identity) const {
  Branch* branch_ = nullptr;
  bool found = false;
  std::queue<Tree::Branch*> queue;
  queue.push(root_);
  while (!queue.empty() && !found) {
    Branch* branch = queue.front();
    if (branch->identity == _identity) {
      found = true;
      branch_ = branch;
    }
    if (!found) {
      queue.pop();
      for (auto child : branch->children) {
        queue.push(child);
      }
    }
  }
  return branch_;
}

bool Tree::HasChild(Tree::Branch* _branch, Node* _identity) const {
  bool has_child = false;
  for (auto child : _branch->children) {
    if (child->identity == _identity) {
      has_child = true;
    }
  }
  return has_child;
}

bool Tree::IsLeaf(Branch* _branch) const {
  return (_branch->children.empty());
}

void Tree::Log(const std::string& _path) const {
  std::ofstream ofstream(_path, std::ios::out | std::ios::trunc);
  // Preamble
  ofstream << "\\documentclass[tikz, border=0pt]{standalone}" << std::endl;
  ofstream << "\\usepackage{tikz}" << std::endl;
  ofstream << "\\usepackage[linguistics]{forest}" << std::endl;
  ofstream << "\\begin{document}" << std::endl;
  ofstream << "\\begin{forest}" << std::endl;
  ofstream << "for tree={l sep+=100pt}";
  // Loop
  std::stack<Branch*> stack;
  stack.push(root_);
  uint8_t current_level = 0;
  while (!stack.empty()) {
    Tree::Branch* branch = stack.top();
    stack.pop();
    for (uint8_t i = branch->level; i < current_level; ++i) {
      ofstream << "]";
    }
    ofstream << std::endl;
    current_level = branch->level;
    for (uint8_t i = 0; i < branch->level; ++i) {
      ofstream << "  ";
    }
    ofstream << "["
             << branch->identity->index()
             << " \\\\ "
             << branch->rx_time_s;
    if (IsLeaf(branch)) {
      ofstream << "]";
    } else {
      for (auto child : branch->children) {
        stack.push(child);
      }
    }
  }
  for (uint8_t i = 0; i < current_level; ++i) {
    ofstream << "]";
  }
  ofstream << std::endl;
  ofstream << "\\end{forest}" << std::endl;
  ofstream << "\\end{document}" << std::endl;
  ofstream.close();
}

}  // namespace collaborate
}  // namespace osse
