#include "nmap/parser.hpp"
#include "nmap/detail/parser_impl.hpp"
#include "definitions.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <sstream>

namespace nmap
{

NmapResult parse(const std::string& data)
{
  std::stringstream ss;
  ss << data;

  boost::property_tree::ptree xml;
  boost::property_tree::read_xml(ss, xml);

  NmapResult result;
  
  result.start_time = xml.get<time_t>("nmaprun.<xmlattr>.start");
  result.end_time = xml.get<time_t>("nmaprun.runstats.finished.<xmlattr>.time");
  
  for (const auto& [name, tree] : xml.get_child("nmaprun")) {
    if (name != "host")
      continue;

    result.hosts.push_back(detail::parse_host(tree));
  }

  return result;
}

} // namespace nmap