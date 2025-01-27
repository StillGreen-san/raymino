#include "cstring_view.hpp"

#include "ostream.hpp" //! needs to be included before catch

#include <catch2/catch_test_macros.hpp>

TEST_CASE("CStringView::CStringView", "[CStringView]")
{
	CHECK("Hello World"_csv == "Hello World");
}

TEST_CASE("CStringView::assumeTerminated", "[CStringView]")
{
	CHECK(CStringView::assumeTerminated("cstr", ""_csv) == "cstr");
	CHECK(CStringView::assumeTerminated({}, "alt"_csv) == "alt");
}
