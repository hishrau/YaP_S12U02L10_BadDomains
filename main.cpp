#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>

class Domain {
public:
    Domain(const std::string& domain_string)
        : domain_(domain_string) { }

    bool operator==(const Domain& domain) const {
        return domain_ == domain.domain_;
    }

    bool IsSubdomain(const Domain& domain) const {
        if (domain.domain_.size() <= domain_.size()) {
            return std::equal(
                domain.domain_.begin(), domain.domain_.end(),
                domain_.begin()
            );
        }
        else {
            return false;
        }
    }

    std::string GetDomain() const {
        return domain_;
    }
private:
    std::string domain_;
};

class DomainChecker {
public:
    template<typename Iterator>
    DomainChecker(Iterator begin, Iterator end)
        : banned_(begin, end)
    {
        SortAndClean();
    }

    bool IsForbidden(const Domain& domain) {
        if (banned_.empty()) {
            return false;
        }
        auto diff_mark = std::upper_bound(banned_.begin(), banned_.end(), domain,
            [](const Domain& tested, const Domain& banned) {
                return tested.GetDomain() < banned.GetDomain();
            }
        );
        if (diff_mark != banned_.begin()) {
            return domain.IsSubdomain(banned_.at(distance(banned_.begin(), diff_mark) - 1));
        }
        else {
            return domain.IsSubdomain(banned_.at(distance(banned_.begin(), diff_mark)));
        }
        return false;
    }

    std::vector<Domain> GetBanned() const {
        return banned_;
    }

private:
    std::vector<Domain> banned_;

    void SortAndClean() {
        std::sort(banned_.begin(), banned_.end(),
            [](const Domain& lhs, const Domain& rhs) {
                return lhs.GetDomain() < rhs.GetDomain();
            }
        );
        auto to_clean = std::unique(banned_.begin(), banned_.end(),
            [](const Domain& lhs, const Domain& rhs) {
                return rhs.IsSubdomain(lhs);
            }
        );
        banned_.erase(to_clean, banned_.end());
    }
};

template<typename Number>
std::vector<Domain> ReadDomains(std::istream& input, Number count) {
    std::vector<Domain> domains;
    domains.reserve(count + 1);
    for (Number i = 0; i < count; ++i) {
        std::string domain_string;
        std::getline(input, domain_string);
        std::reverse(domain_string.begin(), domain_string.end());
        domain_string.push_back('.');
        domains.emplace_back(std::move(domain_string));
    }
    return domains;
}

template<typename Number>
Number ReadNumberOnLine(std::istream& input) {
    std::string line;
    getline(input, line);
    Number num;
    std::istringstream(line) >> num;
    return num;
}

using namespace std::literals;
//void TestReadDomains() {
//    std::stringstream input;
//    input << "2\n"s << "gdz.ru\n"s << "maps.me\n"s;
//    const std::vector<Domain> domains 
//        = ReadDomains(input, ReadNumberOnLine<size_t>(input));
//    assert(domains.size() == 2);
//
//    std::vector<std::string> output;
//    for (auto& d : domains) {
//        std::string s = d.GetDomain();
//        output.push_back(s);
//    }
//    assert(output.size() == 2);
//    assert(output[0] == "ur.zdg."s);
//    assert(output[1] == "em.spam."s);
//    std::cout << "1. ReadDomains() - OK" << std::endl;
//}
//
//void TestDomainCtor() {
//    Domain domain("gdz.ru"s);
//    assert(domain.GetDomain() == "gdz.ru"s);
//    std::cout << "2. Domain Ctor - OK" << std::endl;
//}
//
//void TestEqualDomains() {
//    Domain domain1("ur.zdg."s);
//    Domain domain2("ur.zdg."s);
//    Domain domain3("ur.zdg.a."s);
//    assert(domain1 == domain2);
//    assert(!(domain1 == domain3));
//    std::cout << "3. Equal domains - OK" << std::endl;
//}
//
//void TestIsSubdomain() {
//    Domain domain("ur.zdg."s);
//    Domain subdomain("ur.zdg.a."s);
//    assert(subdomain.IsSubdomain(domain));
//    std::cout << "4. IsSubdomain() - OK" << std::endl;
//}
//
//void TestDomainCheckerCtor() {
//    std::stringstream input;
//    input << "3\n"s << "gdz.ru\n"s << "abc.gdz.ru\n"s << "maps.me\n"s;
//
//    const std::vector<Domain> domains
//        = ReadDomains(input, ReadNumberOnLine<size_t>(input));
//    assert(domains.size() == 3);
//
//    DomainChecker checker(domains.begin(), domains.end());
//    
//    assert(checker.GetBanned().size() == 2);
//    assert(checker.GetBanned().at(1).GetDomain() == "ur.zdg."s);
//    std::cout << "5. DomainChecker Ctor() - OK" << std::endl;
//}
//
//void TestIsForbidden() {
//    std::stringstream input;
//    input << "3\n"s << "gdz.ru\n"s << "abc.gdz.ru\n"s << "maps.me\n"s;
//
//    const std::vector<Domain> domains
//        = ReadDomains(input, ReadNumberOnLine<size_t>(input));
//    DomainChecker checker(domains.begin(), domains.end());
//
//    input << "2\n"s << "freegdz.ru\n"s << "abc.gdz.ru\n"s;
//    const std::vector<Domain> test_domains 
//        = ReadDomains(input, ReadNumberOnLine<size_t>(input));
//    assert(!checker.IsForbidden(test_domains.at(0)));
//    assert(checker.IsForbidden(test_domains.at(1)));
//    std::cout << "6. IsForbidden() - OK" << std::endl;
//}

int main() {
    using namespace std::literals;
    //{
    //    TestReadDomains();
    //    TestDomainCtor();
    //    TestEqualDomains();
    //    TestIsSubdomain();
    //    TestDomainCheckerCtor();
    //    TestIsForbidden();
    //}

    const std::vector<Domain> forbidden_domains = ReadDomains(std::cin, ReadNumberOnLine<size_t>(std::cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(std::cin, ReadNumberOnLine<size_t>(std::cin));
    for (const Domain& domain : test_domains) {
        std::cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << std::endl;
    }
}