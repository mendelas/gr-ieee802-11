#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <ieee802_11/parse_mac_multiout.h>
// pydoc.h is automatically generated in the build directory
#include <parse_mac_multiout_pydoc.h>

void bind_parse_mac_multiout(py::module& m)
{

    using parse_mac_multiout = ::gr::ieee802_11::parse_mac_multiout;


    py::class_<parse_mac_multiout,
               gr::block,
               gr::basic_block,
               std::shared_ptr<parse_mac_multiout>>(
        m, "parse_mac_multiout", D(parse_mac_multiout))

        .def(py::init(&parse_mac_multiout::make),
             py::arg("log") = false,
             py::arg("debug") = false,
             D(parse_mac_multiout, make))


        ;
}
