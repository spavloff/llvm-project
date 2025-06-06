//===--- InlayHints.h --------------------------------------------*- C++-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Support for the proposed "inlay hints" LSP feature.
// The version currently implemented is the one proposed here:
// https://github.com/microsoft/vscode-languageserver-node/pull/609/.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANGD_INLAYHINTS_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANGD_INLAYHINTS_H

#include "Protocol.h"
#include <vector>

namespace clang {
namespace clangd {
class ParsedAST;

struct InlayHintOptions {
  // Minimum height of a code block in lines for a BlockEnd hint to be shown
  // Includes the lines containing the braces
  int HintMinLineLimit = 10;
};

/// Compute and return inlay hints for a file.
/// If RestrictRange is set, return only hints whose location is in that range.
std::vector<InlayHint> inlayHints(ParsedAST &AST,
                                  std::optional<Range> RestrictRange,
                                  InlayHintOptions HintOptions = {});

} // namespace clangd
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANGD_INLAYHINTS_H
