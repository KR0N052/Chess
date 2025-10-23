# CMake generated Testfile for 
# Source directory: C:/Users/melch/OneDrive/Desktop/cpp_ketto
# Build directory: C:/Users/melch/OneDrive/Desktop/cpp_ketto/out/build/x64-Debug
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(PawnTests "C:/Users/melch/OneDrive/Desktop/cpp_ketto/out/build/x64-Debug/basic_piece_tests.exe" "p")
set_tests_properties(PawnTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/melch/OneDrive/Desktop/cpp_ketto/CMakeLists.txt;81;add_test;C:/Users/melch/OneDrive/Desktop/cpp_ketto/CMakeLists.txt;0;")
add_test(RookTests "C:/Users/melch/OneDrive/Desktop/cpp_ketto/out/build/x64-Debug/basic_piece_tests.exe" "r")
set_tests_properties(RookTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/melch/OneDrive/Desktop/cpp_ketto/CMakeLists.txt;82;add_test;C:/Users/melch/OneDrive/Desktop/cpp_ketto/CMakeLists.txt;0;")
add_test(KnightTests "C:/Users/melch/OneDrive/Desktop/cpp_ketto/out/build/x64-Debug/basic_piece_tests.exe" "k")
set_tests_properties(KnightTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/melch/OneDrive/Desktop/cpp_ketto/CMakeLists.txt;83;add_test;C:/Users/melch/OneDrive/Desktop/cpp_ketto/CMakeLists.txt;0;")
add_test(BishopTests "C:/Users/melch/OneDrive/Desktop/cpp_ketto/out/build/x64-Debug/basic_piece_tests.exe" "b")
set_tests_properties(BishopTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/melch/OneDrive/Desktop/cpp_ketto/CMakeLists.txt;84;add_test;C:/Users/melch/OneDrive/Desktop/cpp_ketto/CMakeLists.txt;0;")
add_test(QueenTests "C:/Users/melch/OneDrive/Desktop/cpp_ketto/out/build/x64-Debug/basic_piece_tests.exe" "q")
set_tests_properties(QueenTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/melch/OneDrive/Desktop/cpp_ketto/CMakeLists.txt;85;add_test;C:/Users/melch/OneDrive/Desktop/cpp_ketto/CMakeLists.txt;0;")
add_test(KingTests "C:/Users/melch/OneDrive/Desktop/cpp_ketto/out/build/x64-Debug/basic_piece_tests.exe" "k")
set_tests_properties(KingTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/melch/OneDrive/Desktop/cpp_ketto/CMakeLists.txt;86;add_test;C:/Users/melch/OneDrive/Desktop/cpp_ketto/CMakeLists.txt;0;")
add_test(CastlingTests "C:/Users/melch/OneDrive/Desktop/cpp_ketto/out/build/x64-Debug/special_moves_tests.exe" "castle")
set_tests_properties(CastlingTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/melch/OneDrive/Desktop/cpp_ketto/CMakeLists.txt;90;add_test;C:/Users/melch/OneDrive/Desktop/cpp_ketto/CMakeLists.txt;0;")
add_test(EnPassantTests "C:/Users/melch/OneDrive/Desktop/cpp_ketto/out/build/x64-Debug/special_moves_tests.exe" "enpassant")
set_tests_properties(EnPassantTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/melch/OneDrive/Desktop/cpp_ketto/CMakeLists.txt;91;add_test;C:/Users/melch/OneDrive/Desktop/cpp_ketto/CMakeLists.txt;0;")
add_test(PromotionTests "C:/Users/melch/OneDrive/Desktop/cpp_ketto/out/build/x64-Debug/special_moves_tests.exe" "promotion")
set_tests_properties(PromotionTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/melch/OneDrive/Desktop/cpp_ketto/CMakeLists.txt;92;add_test;C:/Users/melch/OneDrive/Desktop/cpp_ketto/CMakeLists.txt;0;")
add_test(CheckMateTest "C:/Users/melch/OneDrive/Desktop/cpp_ketto/out/build/x64-Debug/global_tests.exe" "checkmate")
set_tests_properties(CheckMateTest PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/melch/OneDrive/Desktop/cpp_ketto/CMakeLists.txt;96;add_test;C:/Users/melch/OneDrive/Desktop/cpp_ketto/CMakeLists.txt;0;")
add_test(AttackedTest "C:/Users/melch/OneDrive/Desktop/cpp_ketto/out/build/x64-Debug/global_tests.exe" "attacked")
set_tests_properties(AttackedTest PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/melch/OneDrive/Desktop/cpp_ketto/CMakeLists.txt;97;add_test;C:/Users/melch/OneDrive/Desktop/cpp_ketto/CMakeLists.txt;0;")
add_test(StalemateTest "C:/Users/melch/OneDrive/Desktop/cpp_ketto/out/build/x64-Debug/global_tests.exe" "stalemate")
set_tests_properties(StalemateTest PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/melch/OneDrive/Desktop/cpp_ketto/CMakeLists.txt;98;add_test;C:/Users/melch/OneDrive/Desktop/cpp_ketto/CMakeLists.txt;0;")
add_test(WouldBeInCheckTest "C:/Users/melch/OneDrive/Desktop/cpp_ketto/out/build/x64-Debug/global_tests.exe" "wouldbeincheck")
set_tests_properties(WouldBeInCheckTest PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/melch/OneDrive/Desktop/cpp_ketto/CMakeLists.txt;99;add_test;C:/Users/melch/OneDrive/Desktop/cpp_ketto/CMakeLists.txt;0;")
add_test(MCTSTest "C:/Users/melch/OneDrive/Desktop/cpp_ketto/out/build/x64-Debug/agent_tests.exe" "mcts")
set_tests_properties(MCTSTest PROPERTIES  PASS_REGULAR_EXPRESSION ".*" _BACKTRACE_TRIPLES "C:/Users/melch/OneDrive/Desktop/cpp_ketto/CMakeLists.txt;103;add_test;C:/Users/melch/OneDrive/Desktop/cpp_ketto/CMakeLists.txt;0;")
