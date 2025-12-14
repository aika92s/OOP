#ifndef MINESWEEPERLAB_OBJECTFACTORY_H
#define MINESWEEPERLAB_OBJECTFACTORY_H
#include <map>
#include <functional>
#include <memory>
#include <vector>

template <class IdentifierType, class AbstractProduct>
class DefaultFactoryError {
public:
    class Exception : public std::exception {
        const char* what() const noexcept override { return "Unknown Object Type in Factory"; }
    };

    static std::unique_ptr<AbstractProduct> OnUnknownType(const IdentifierType& id) {
        throw Exception();
    }
};

template <
    class AbstractProduct,
    class IdentifierType,
    class Creator = std::function<std::unique_ptr<AbstractProduct>()>,
    class ErrorPolicy = DefaultFactoryError<IdentifierType, AbstractProduct>
>
class Factory : public ErrorPolicy {
public:

    bool Register(const IdentifierType& id, Creator creator) {
        return associations_.insert({id, creator}).second;
    }

    bool Unregister(const IdentifierType& id) {
        return associations_.erase(id) == 1;
    }

    std::unique_ptr<AbstractProduct> CreateObject(const IdentifierType& id) {
        auto i = associations_.find(id);
        if (i != associations_.end()) {
            return (i->second)();
        }

        return ErrorPolicy::OnUnknownType(id);
    }

private:
    using AssocMap = std::map<IdentifierType, Creator>;
    AssocMap associations_;
};


#endif //MINESWEEPERLAB_OBJECTFACTORY_H