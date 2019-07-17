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

#ifndef LIBS_COLLABORATE_INCLUDE_COLLABORATE_TREE_H_
#define LIBS_COLLABORATE_INCLUDE_COLLABORATE_TREE_H_

#include <cstdint>
#include <string>
#include <vector>

#include "collaborate/node.h"

namespace osse {
namespace collaborate {

/// @class Tree
/// @brief A tree
/// @image html raster/tree.png width=500
/// @image latex vector/tree.pdf width=\textwidth
class Tree {
 public:
  /// @brief A node of the tree
  typedef struct Branch {
    /// @brief Branch directly above this one
    Branch* parent;
    /// @brief A list of branches originating from this one
    std::vector<Branch*> children;
    /// @brief Level of the current branch in the tree
    uint16_t level;
    /// @brief Corresponding satellite node
    Node* identity;
    /// @brief Message received time (seconds)
    uint64_t rx_time_s;
  } Branch;
  /// @brief Constructor
  /// @param[in] _root Root node
  /// @param[in] _height Maximum levels in the tree
  /// @param[in] _target Target node
  Tree(Node* _root, const uint16_t& _height, Node* _target);
  /// @brief Destructor
  ~Tree();
  /// @brief Deletes a subtree
  /// @param[in] _branch Root of the substree
  void DeleteSubtree(Branch* _branch);
  /// @brief Creates a root node
  /// @param[in] _root_identity The root identity
  /// @returns A root node
  Branch* CreateRoot(Node* _root_identity);
  /// @brief Add a child to the tree
  /// @param[in] _parent Parent branch
  /// @param[in] _identity New satellite node to add
  /// @param[in] _rx_time_s Message receive time of the new node (seconds)
  /// @returns New branch child
  Branch* AddChild(Branch* _parent,
                   Node* _identity,
                   const uint64_t& _rx_time_s);
  /// @brief Obtain list of direct parents (back to root node)
  /// @param[in] _branch Lowest node in the ancestry
  /// @returns List of direct parents, back to the root
  std::vector<Branch*> Ancestry(Branch* _branch) const;
  /// @brief Perform a BFS on the tree to find a node with specified rx time
  /// @param[in] _identity Node to search for
  /// @param[in] _rx_time_s Message receive time of the new node (seconds)
  /// @returns Specific branch found
  Branch* SearchSpecific(Node* _identity, const uint64_t& _rx_time_s) const;
  /// @brief Perform a BFS on the tree to find a node
  /// @param[in] _identity Node to search for
  /// @returns First branch found from top down and left to right
  Branch* BreadthFirstSearch(Node* _identity) const;
  /// @brief Determine if a node is a current child of a node
  /// @param[in] _branch Tree node parent
  /// @param[in] _identity Node child
  /// @returns Whether the child exists
  bool HasChild(Branch* _branch, Node* _identity) const;
  /// @brief Determine if a node has no children
  /// @param[in] _branch Tree node
  /// @returns Whether it is a leaf
  bool IsLeaf(Branch* _branch) const;
  /// @brief Write the tree to a LaTeX Tikz file
  /// @param[in] _path Path to the log file
  void Log(const std::string& _path) const;
  /// @brief Set the root tree node
  /// @param _root Root tree node
  void set_root(Branch* _root) {root_ = _root;}
  /// @brief Set the height
  /// @param[in] _height Height
  void set_height(const uint16_t& _height) {height_ = _height;}
  /// @brief Set the target node
  /// @param _target Target node
  void set_target(Node* _target) {target_ = _target;}
  /// @brief Get the root tree node
  /// @returns root_ Root tree node
  Branch* root() const {return root_;}
  /// @brief Get the number of tree nodes in the tree
  /// @returns size_ Number of tree nodes in the tree
  const uint64_t& size() const {return size_;}
  /// @brief Get the height
  /// @returns height_ Height
  const uint16_t& height() const {return height_;}
  /// @brief Get the target node
  /// @returns target_ Target node
  Node* target() const {return target_;}

 private:
  /// @brief Root tree node
  Branch* root_;
  /// @brief Number of tree nodes in the tree
  uint64_t size_;
  /// @brief Height
  uint16_t height_;
  /// @brief Target node;
  Node* target_;
};

}  // namespace collaborate
}  // namespace osse

#endif  // LIBS_COLLABORATE_INCLUDE_COLLABORATE_TREE_H_
